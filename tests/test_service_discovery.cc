#include "autoli/streams/service_discovery.h"
#include "autoli/iomanager.h"
#include "autoli/rock/rock_stream.h"
#include "autoli/log.h"
#include "autoli/worker.h"

autoli::ZKServiceDiscovery::ptr zksd(new autoli::ZKServiceDiscovery("127.0.0.1:21812"));
autoli::RockSDLoadBalance::ptr rsdlb(new autoli::RockSDLoadBalance(zksd));

static autoli::Logger::ptr g_logger = AUTOLI_LOG_ROOT();

std::atomic<uint32_t> s_id;
void on_timer() {
    g_logger->setLevel(autoli::LogLevel::INFO);
    //AUTOLI_LOG_INFO(g_logger) << "on_timer";
    autoli::RockRequest::ptr req(new autoli::RockRequest);
    req->setSn(++s_id);
    req->setCmd(100);
    req->setBody("hello");

    auto rt = rsdlb->request("autoli.top", "blog", req, 1000);
    if(!rt->response) {
        if(req->getSn() % 50 == 0) {
            AUTOLI_LOG_ERROR(g_logger) << "invalid response: " << rt->toString();
        }
    } else {
        if(req->getSn() % 1000 == 0) {
            AUTOLI_LOG_INFO(g_logger) << rt->toString();
        }
    }
}

void run() {
    zksd->setSelfInfo("127.0.0.1:2222");
    zksd->setSelfData("aaaa");

    std::unordered_map<std::string, std::unordered_map<std::string,std::string> > confs;
    confs["autoli.top"]["blog"] = "fair";
    rsdlb->start(confs);
    //AUTOLI_LOG_INFO(g_logger) << "on_timer---";

    autoli::IOManager::GetThis()->addTimer(1, on_timer, true);
}

int main(int argc, char** argv) {
    autoli::WorkerMgr::GetInstance()->init({
        {"service_io", {
            {"thread_num", "1"}
        }}
    });
    autoli::IOManager iom(1);
    iom.addTimer(1000, [](){
            std::cout << rsdlb->statusString() << std::endl;
    }, true);
    iom.schedule(run);
    return 0;
}
