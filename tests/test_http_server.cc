#include "../autoli/http/http_server.h"
#include "../autoli/log.h"

static autoli::Logger::ptr g_logger = AUTOLI_LOG_ROOT();

#define XX(...) #__VA_ARGS__


autoli::IOManager::ptr worker;
void run() {
    g_logger->setLevel(autoli::LogLevel::INFO);
    //autoli::http::HttpServer::ptr server(new autoli::http::HttpServer(true, worker.get(), autoli::IOManager::GetThis()));
    //autoli::http::HttpServer::ptr server(new autoli::http::HttpServer(true));
    autoli::http::HttpServer::ptr server(new autoli::http::HttpServer);
    autoli::Address::ptr addr = autoli::Address::LookupAnyIPAddress("0.0.0.0:8020");
    while(!server->bind(addr)) {
        sleep(2);
    }
    
    auto sd = server->getServletDispatch();
    sd->addServlet("/autoli/xx", [](autoli::http::HttpRequest::ptr req
                ,autoli::http::HttpResponse::ptr rsp
                ,autoli::http::HttpSession::ptr session) {
            rsp->setBody(req->toString());
            return 0;
    });

    sd->addGlobServlet("/autoli/*", [](autoli::http::HttpRequest::ptr req
                ,autoli::http::HttpResponse::ptr rsp
                ,autoli::http::HttpSession::ptr session) {
            rsp->setBody("Glob:\r\n" + req->toString());
            return 0;
    });
    //浏览器输入http://192.168.1.9:8020/autoli/* http://192.168.1.9:8020/autoli/xxdasas

//     sd->addGlobServlet("/autolix/*", [](autoli::http::HttpRequest::ptr req
//                 ,autoli::http::HttpResponse::ptr rsp
//                 ,autoli::http::HttpSession::ptr session) {
//             rsp->setBody(XX(<html>
// <head><title>404 Not Found</title></head>
// <body>
// <center><h1>404 Not Found</h1></center>
// <hr><center>nginx/1.16.0</center>
// </body>
// </html>
// <!-- a padding to disable MSIE and Chrome friendly error page -->
// <!-- a padding to disable MSIE and Chrome friendly error page -->
// <!-- a padding to disable MSIE and Chrome friendly error page -->
// <!-- a padding to disable MSIE and Chrome friendly error page -->
// <!-- a padding to disable MSIE and Chrome friendly error page -->
// <!-- a padding to disable MSIE and Chrome friendly error page -->
// ));
//             return 0;
//     });

    server->start();
}

int main(int argc, char** argv) {
    autoli::IOManager iom(2);
    // autoli::IOManager iom(1, true, "main");
    // worker.reset(new autoli::IOManager(3, false, "worker"));
    iom.schedule(run);
    return 0;
}
