#include "../autoli/http/http_server.h"
#include "../autoli/log.h"

autoli::Logger::ptr g_logger = AUTOLI_LOG_ROOT();
autoli::IOManager::ptr worker;
void run() {
    g_logger->setLevel(autoli::LogLevel::INFO);
    autoli::Address::ptr addr = autoli::Address::LookupAnyIPAddress("0.0.0.0:8020");
    if(!addr) {
        AUTOLI_LOG_ERROR(g_logger) << "get address error";
        return;
    }

    autoli::http::HttpServer::ptr http_server(new autoli::http::HttpServer(true, worker.get()));
    //autoli::http::HttpServer::ptr http_server(new autoli::http::HttpServer(true));
    bool ssl = false;
    while(!http_server->bind(addr, ssl)) {
        AUTOLI_LOG_ERROR(g_logger) << "bind " << *addr << " fail";
        sleep(1);
    }

    if(ssl) {
        //http_server->loadCertificates("/home/apps/soft/autoli/keys/server.crt", "/home/apps/soft/autoli/keys/server.key");
    }

    http_server->start();
}

int main(int argc, char** argv) {
    autoli::IOManager iom(1);
    worker.reset(new autoli::IOManager(4, false));
    iom.schedule(run);
    return 0;
}
