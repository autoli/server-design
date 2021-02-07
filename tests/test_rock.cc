#include "autoli/autoli.h"
#include "autoli/rock/rock_stream.h"

static autoli::Logger::ptr g_logger = AUTOLI_LOG_ROOT();

autoli::RockConnection::ptr conn(new autoli::RockConnection);
void run() {
    conn->setAutoConnect(true);
    autoli::Address::ptr addr = autoli::Address::LookupAny("127.0.0.1:8061");
    if(!conn->connect(addr)) {
        AUTOLI_LOG_INFO(g_logger) << "connect " << *addr << " false";
    }
    conn->start();

    autoli::IOManager::GetThis()->addTimer(1000, [](){
        autoli::RockRequest::ptr req(new autoli::RockRequest);
        static uint32_t s_sn = 0;
        req->setSn(++s_sn);
        req->setCmd(100);
        req->setBody("hello world sn=" + std::to_string(s_sn));

        auto rsp = conn->request(req, 300);
        if(rsp->response) {
            AUTOLI_LOG_INFO(g_logger) << rsp->response->toString();
        } else {
            AUTOLI_LOG_INFO(g_logger) << "error result=" << rsp->result;
        }
    }, true);
}

int main(int argc, char** argv) {
    autoli::IOManager iom(1);
    iom.schedule(run);
    return 0;
}
