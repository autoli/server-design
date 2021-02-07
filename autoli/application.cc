#include "application.h"

#include <unistd.h>
#include <signal.h>

#include "autoli/tcp_server.h"
#include "autoli/daemon.h"
#include "autoli/config.h"
#include "autoli/env.h"
#include "autoli/log.h"
#include "autoli/module.h"
#include "autoli/rock/rock_stream.h"
#include "autoli/worker.h"
#include "autoli/http/ws_server.h"
#include "autoli/rock/rock_server.h"
#include "autoli/ns/name_server_module.h"
#include "autoli/db/fox_thread.h"
#include "autoli/db/redis.h"

namespace autoli {

static autoli::Logger::ptr g_logger = AUTOLI_LOG_NAME("system");

static autoli::ConfigVar<std::string>::ptr g_server_work_path =
    autoli::Config::Lookup("server.work_path"
            ,std::string("/apps/work/autoli")
            , "server work path");

static autoli::ConfigVar<std::string>::ptr g_server_pid_file =
    autoli::Config::Lookup("server.pid_file"
            ,std::string("autoli.pid")
            , "server pid file");

static autoli::ConfigVar<std::string>::ptr g_service_discovery_zk =
    autoli::Config::Lookup("service_discovery.zk"
            ,std::string("")
            , "service discovery zookeeper");


static autoli::ConfigVar<std::vector<TcpServerConf> >::ptr g_servers_conf
    = autoli::Config::Lookup("servers", std::vector<TcpServerConf>(), "http server config");

Application* Application::s_instance = nullptr;

Application::Application() {
    s_instance = this;
}

bool Application::init(int argc, char** argv) {
    m_argc = argc;
    m_argv = argv;

    autoli::EnvMgr::GetInstance()->addHelp("s", "start with the terminal");
    autoli::EnvMgr::GetInstance()->addHelp("d", "run as daemon");
    autoli::EnvMgr::GetInstance()->addHelp("c", "conf path default: ./conf");
    autoli::EnvMgr::GetInstance()->addHelp("p", "print help");

    bool is_print_help = false;
    if(!autoli::EnvMgr::GetInstance()->init(argc, argv)) {
        is_print_help = true;
    }

    if(autoli::EnvMgr::GetInstance()->has("p")) {
        is_print_help = true;
    }

    std::string conf_path = autoli::EnvMgr::GetInstance()->getConfigPath();
    AUTOLI_LOG_INFO(g_logger) << "load conf path:" << conf_path;
    autoli::Config::LoadFromConfDir(conf_path);

    ModuleMgr::GetInstance()->init();
    std::vector<Module::ptr> modules;
    ModuleMgr::GetInstance()->listAll(modules);

    for(auto i : modules) {
        i->onBeforeArgsParse(argc, argv);
    }

    if(is_print_help) {
        autoli::EnvMgr::GetInstance()->printHelp();
        return false;
    }

    for(auto i : modules) {
        i->onAfterArgsParse(argc, argv);
    }
    modules.clear();

    int run_type = 0;
    if(autoli::EnvMgr::GetInstance()->has("s")) {
        run_type = 1;
    }
    if(autoli::EnvMgr::GetInstance()->has("d")) {
        run_type = 2;
    }

    if(run_type == 0) {
        autoli::EnvMgr::GetInstance()->printHelp();
        return false;
    }

    std::string pidfile = g_server_work_path->getValue()
                                + "/" + g_server_pid_file->getValue();
    if(autoli::FSUtil::IsRunningPidfile(pidfile)) {
        AUTOLI_LOG_ERROR(g_logger) << "server is running:" << pidfile;
        return false;
    }

    if(!autoli::FSUtil::Mkdir(g_server_work_path->getValue())) {
        AUTOLI_LOG_FATAL(g_logger) << "create work path [" << g_server_work_path->getValue()
            << " errno=" << errno << " errstr=" << strerror(errno);
        return false;
    }
    return true;
}

bool Application::run() {
    bool is_daemon = autoli::EnvMgr::GetInstance()->has("d");
    return start_daemon(m_argc, m_argv,
            std::bind(&Application::main, this, std::placeholders::_1,
                std::placeholders::_2), is_daemon);
}

int Application::main(int argc, char** argv) {
    signal(SIGPIPE, SIG_IGN);
    AUTOLI_LOG_INFO(g_logger) << "main";
    std::string conf_path = autoli::EnvMgr::GetInstance()->getConfigPath();
    autoli::Config::LoadFromConfDir(conf_path, true);
    {
        std::string pidfile = g_server_work_path->getValue()
                                    + "/" + g_server_pid_file->getValue();
        std::ofstream ofs(pidfile);
        if(!ofs) {
            AUTOLI_LOG_ERROR(g_logger) << "open pidfile " << pidfile << " failed";
            return false;
        }
        ofs << getpid();
    }

    m_mainIOManager.reset(new autoli::IOManager(1, true, "main"));
    m_mainIOManager->schedule(std::bind(&Application::run_fiber, this));
    m_mainIOManager->addTimer(2000, [](){
            //AUTOLI_LOG_INFO(g_logger) << "hello";
    }, true);
    m_mainIOManager->stop();
    return 0;
}

int Application::run_fiber() {
    std::vector<Module::ptr> modules;
    ModuleMgr::GetInstance()->listAll(modules);
    bool has_error = false;
    for(auto& i : modules) {
        if(!i->onLoad()) {
            AUTOLI_LOG_ERROR(g_logger) << "module name="
                << i->getName() << " version=" << i->getVersion()
                << " filename=" << i->getFilename();
            has_error = true;
        }
    }
    if(has_error) {
        _exit(0);
    }

    autoli::WorkerMgr::GetInstance()->init();
    FoxThreadMgr::GetInstance()->init();
    FoxThreadMgr::GetInstance()->start();
    RedisMgr::GetInstance();

    auto http_confs = g_servers_conf->getValue();
    std::vector<TcpServer::ptr> svrs;
    for(auto& i : http_confs) {
        AUTOLI_LOG_DEBUG(g_logger) << std::endl << LexicalCast<TcpServerConf, std::string>()(i);

        std::vector<Address::ptr> address;
        for(auto& a : i.address) {
            size_t pos = a.find(":");
            if(pos == std::string::npos) {
                //AUTOLI_LOG_ERROR(g_logger) << "invalid address: " << a;
                address.push_back(UnixAddress::ptr(new UnixAddress(a)));
                continue;
            }
            int32_t port = atoi(a.substr(pos + 1).c_str());
            //127.0.0.1
            auto addr = autoli::IPAddress::Create(a.substr(0, pos).c_str(), port);
            if(addr) {
                address.push_back(addr);
                continue;
            }
            std::vector<std::pair<Address::ptr, uint32_t> > result;
            if(autoli::Address::GetInterfaceAddresses(result,
                                        a.substr(0, pos))) {
                for(auto& x : result) {
                    auto ipaddr = std::dynamic_pointer_cast<IPAddress>(x.first);
                    if(ipaddr) {
                        ipaddr->setPort(atoi(a.substr(pos + 1).c_str()));
                    }
                    address.push_back(ipaddr);
                }
                continue;
            }

            auto aaddr = autoli::Address::LookupAny(a);
            if(aaddr) {
                address.push_back(aaddr);
                continue;
            }
            AUTOLI_LOG_ERROR(g_logger) << "invalid address: " << a;
            _exit(0);
        }
        IOManager* accept_worker = autoli::IOManager::GetThis();
        IOManager* io_worker = autoli::IOManager::GetThis();
        IOManager* process_worker = autoli::IOManager::GetThis();
        if(!i.accept_worker.empty()) {
            accept_worker = autoli::WorkerMgr::GetInstance()->getAsIOManager(i.accept_worker).get();
            if(!accept_worker) {
                AUTOLI_LOG_ERROR(g_logger) << "accept_worker: " << i.accept_worker
                    << " not exists";
                _exit(0);
            }
        }
        if(!i.io_worker.empty()) {
            io_worker = autoli::WorkerMgr::GetInstance()->getAsIOManager(i.io_worker).get();
            if(!io_worker) {
                AUTOLI_LOG_ERROR(g_logger) << "io_worker: " << i.io_worker
                    << " not exists";
                _exit(0);
            }
        }
        if(!i.process_worker.empty()) {
            process_worker = autoli::WorkerMgr::GetInstance()->getAsIOManager(i.process_worker).get();
            if(!process_worker) {
                AUTOLI_LOG_ERROR(g_logger) << "process_worker: " << i.process_worker
                    << " not exists";
                _exit(0);
            }
        }

        TcpServer::ptr server;
        if(i.type == "http") {
            server.reset(new autoli::http::HttpServer(i.keepalive,
                            process_worker, io_worker, accept_worker));
        } else if(i.type == "ws") {
            server.reset(new autoli::http::WSServer(
                            process_worker, io_worker, accept_worker));
        } else if(i.type == "rock") {
            server.reset(new autoli::RockServer("rock",
                            process_worker, io_worker, accept_worker));
        } else if(i.type == "nameserver") {
            server.reset(new autoli::RockServer("nameserver",
                            process_worker, io_worker, accept_worker));
            ModuleMgr::GetInstance()->add(std::make_shared<autoli::ns::NameServerModule>());
        } else {
            AUTOLI_LOG_ERROR(g_logger) << "invalid server type=" << i.type
                << LexicalCast<TcpServerConf, std::string>()(i);
            _exit(0);
        }
        if(!i.name.empty()) {
            server->setName(i.name);
        }
        std::vector<Address::ptr> fails;
        if(!server->bind(address, fails, i.ssl)) {
            for(auto& x : fails) {
                AUTOLI_LOG_ERROR(g_logger) << "bind address fail:"
                    << *x;
            }
            _exit(0);
        }
        if(i.ssl) {
            if(!server->loadCertificates(i.cert_file, i.key_file)) {
                AUTOLI_LOG_ERROR(g_logger) << "loadCertificates fail, cert_file="
                    << i.cert_file << " key_file=" << i.key_file;
            }
        }
        server->setConf(i);
        //server->start();
        m_servers[i.type].push_back(server);
        svrs.push_back(server);
    }

    if(!g_service_discovery_zk->getValue().empty()) {
        m_serviceDiscovery.reset(new ZKServiceDiscovery(g_service_discovery_zk->getValue()));
        m_rockSDLoadBalance.reset(new RockSDLoadBalance(m_serviceDiscovery));

        std::vector<TcpServer::ptr> svrs;
        if(!getServer("http", svrs)) {
            m_serviceDiscovery->setSelfInfo(autoli::GetIPv4() + ":0:" + autoli::GetHostName());
        } else {
            std::string ip_and_port;
            for(auto& i : svrs) {
                auto socks = i->getSocks();
                for(auto& s : socks) {
                    auto addr = std::dynamic_pointer_cast<IPv4Address>(s->getLocalAddress());
                    if(!addr) {
                        continue;
                    }
                    auto str = addr->toString();
                    if(str.find("127.0.0.1") == 0) {
                        continue;
                    }
                    if(str.find("0.0.0.0") == 0) {
                        ip_and_port = autoli::GetIPv4() + ":" + std::to_string(addr->getPort());
                        break;
                    } else {
                        ip_and_port = addr->toString();
                    }
                }
                if(!ip_and_port.empty()) {
                    break;
                }
            }
            m_serviceDiscovery->setSelfInfo(ip_and_port + ":" + autoli::GetHostName());
        }
    }

    for(auto& i : modules) {
        i->onServerReady();
    }

    for(auto& i : svrs) {
        i->start();
    }

    if(m_rockSDLoadBalance) {
        m_rockSDLoadBalance->start();
    }

    for(auto& i : modules) {
        i->onServerUp();
    }
    //ZKServiceDiscovery::ptr m_serviceDiscovery;
    //RockSDLoadBalance::ptr m_rockSDLoadBalance;
    //autoli::ZKServiceDiscovery::ptr zksd(new autoli::ZKServiceDiscovery("127.0.0.1:21811"));
    //zksd->registerServer("blog", "chat", autoli::GetIPv4() + ":8090", "xxx");
    //zksd->queryServer("blog", "chat");
    //zksd->setSelfInfo(autoli::GetIPv4() + ":8090");
    //zksd->setSelfData("vvv");
    //static RockSDLoadBalance::ptr rsdlb(new RockSDLoadBalance(zksd));
    //rsdlb->start();
    return 0;
}

bool Application::getServer(const std::string& type, std::vector<TcpServer::ptr>& svrs) {
    auto it = m_servers.find(type);
    if(it == m_servers.end()) {
        return false;
    }
    svrs = it->second;
    return true;
}

void Application::listAllServer(std::map<std::string, std::vector<TcpServer::ptr> >& servers) {
    servers = m_servers;
}

}
