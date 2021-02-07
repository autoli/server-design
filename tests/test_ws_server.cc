#include "autoli/http/ws_server.h"
#include "autoli/log.h"

static autoli::Logger::ptr g_logger = AUTOLI_LOG_ROOT();

void run() {
    autoli::http::WSServer::ptr server(new autoli::http::WSServer);
    autoli::Address::ptr addr = autoli::Address::LookupAnyIPAddress("0.0.0.0:8020");
    if(!addr) {
        AUTOLI_LOG_ERROR(g_logger) << "get address error";
        return;
    }
    auto fun = [](autoli::http::HttpRequest::ptr header
                  ,autoli::http::WSFrameMessage::ptr msg
                  ,autoli::http::WSSession::ptr session) {
        session->sendMessage(msg);
        return 0;
    };

    server->getWSServletDispatch()->addServlet("/autoli", fun);
    while(!server->bind(addr)) {
        AUTOLI_LOG_ERROR(g_logger) << "bind " << *addr << " fail";
        sleep(1);
    }
    server->start();
}

int main(int argc, char** argv) {
    autoli::IOManager iom(2);
    iom.schedule(run);
    return 0;
}
