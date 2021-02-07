#include "autoli/autoli.h"
#include "autoli/ns/ns_protocol.h"
#include "autoli/ns/ns_client.h"

static autoli::Logger::ptr g_logger = AUTOLI_LOG_ROOT();

int type = 0;

void run() {
    g_logger->setLevel(autoli::LogLevel::INFO);
    auto addr = autoli::IPAddress::Create("127.0.0.1", 8072);
    //if(!conn->connect(addr)) {
    //    AUTOLI_LOG_ERROR(g_logger) << "connect to: " << *addr << " fail";
    //    return;
    //}
    if(type == 0) {
        for(int i = 0; i < 5000; ++i) {
            autoli::RockConnection::ptr conn(new autoli::RockConnection);
            conn->connect(addr);
            autoli::IOManager::GetThis()->addTimer(3000, [conn, i](){
                    autoli::RockRequest::ptr req(new autoli::RockRequest);
                    req->setCmd((int)autoli::ns::NSCommand::REGISTER);
                    auto rinfo = std::make_shared<autoli::ns::RegisterRequest>();
                    auto info = rinfo->add_infos();
                    info->set_domain(std::to_string(rand() % 2) + "domain.com");
                    info->add_cmds(rand() % 2 + 100);
                    info->add_cmds(rand() % 2 + 200);
                    info->mutable_node()->set_ip("127.0.0.1");
                    info->mutable_node()->set_port(1000 + i);
                    info->mutable_node()->set_weight(100);
                    req->setAsPB(*rinfo);

                    auto rt = conn->request(req, 100);
                    AUTOLI_LOG_INFO(g_logger) << "[result="
                        << rt->result << " response="
                        << (rt->response ? rt->response->toString() : "null")
                        << "]";
            }, true);
            conn->start();
        }
    } else {
        for(int i = 0; i < 1000; ++i) {
            autoli::ns::NSClient::ptr nsclient(new autoli::ns::NSClient);
            nsclient->init();
            nsclient->addQueryDomain(std::to_string(i % 2) + "domain.com");
            nsclient->connect(addr);
            nsclient->start();
            AUTOLI_LOG_INFO(g_logger) << "NSClient start: i=" << i;

            if(i == 0) {
                //autoli::IOManager::GetThis()->addTimer(1000, [nsclient](){
                //    auto domains = nsclient->getDomains();
                //    domains->dump(std::cout, "    ");
                //}, true);
            }
        }

        //conn->setConnectCb([](autoli::AsyncSocketStream::ptr ss) {
        //    auto conn = std::dynamic_pointer_cast<autoli::RockConnection>(ss);
        //    autoli::RockRequest::ptr req(new autoli::RockRequest);
        //    req->setCmd((int)autoli::ns::NSCommand::QUERY);
        //    auto rinfo = std::make_shared<autoli::ns::QueryRequest>();
        //    rinfo->add_domains("0domain.com");
        //    req->setAsPB(*rinfo);
        //    auto rt = conn->request(req, 1000);
        //    AUTOLI_LOG_INFO(g_logger) << "[result="
        //        << rt->result << " response="
        //        << (rt->response ? rt->response->toString() : "null")
        //        << "]";
        //    return true;
        //});

        //conn->setNotifyHandler([](autoli::RockNotify::ptr nty,autoli::RockStream::ptr stream){
        //        auto nm = nty->getAsPB<autoli::ns::NotifyMessage>();
        //        if(!nm) {
        //            AUTOLI_LOG_ERROR(g_logger) << "invalid notify message";
        //            return true;
        //        }
        //        AUTOLI_LOG_INFO(g_logger) << autoli::PBToJsonString(*nm);
        //        return true;
        //});
    }
}

int main(int argc, char** argv) {
    if(argc > 1) {
        type = 1;
    }
    autoli::IOManager iom(5);
    iom.schedule(run);
    return 0;
}
