#include "../autoli/http/http.h"
#include "../autoli/log.h"

void test_request() {
    autoli::http::HttpRequest::ptr req(new autoli::http::HttpRequest);
    req->setHeader("host" , "www.sylar.top");
    req->setBody("hello autoli");
    req->dump(std::cout) << std::endl;
}

void test_response() {
    autoli::http::HttpResponse::ptr rsp(new autoli::http::HttpResponse);
    rsp->setHeader("X-X", "autoli");
    rsp->setBody("hello autoli");
    rsp->setStatus((autoli::http::HttpStatus)400);
    rsp->setClose(false);

    rsp->dump(std::cout) << std::endl;
}

int main(int argc, char** argv) {
    test_request();
    test_response();
    return 0;
}
