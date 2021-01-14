#include <iostream>
#include "autoli/http/http_connection.h"
#include "autoli/log.h"
#include "autoli/iomanager.h"
#include "autoli/http/http_parser.h"
#include "autoli/streams/zlib_stream.h"
#include <fstream>

static autoli::Logger::ptr g_logger = AUTOLI_LOG_ROOT();

void test_pool() {
    autoli::http::HttpConnectionPool::ptr pool(new autoli::http::HttpConnectionPool(
                "www.sylar.top", "", 80, false, 10, 1000 * 30, 5));

    autoli::IOManager::GetThis()->addTimer(1000, [pool](){
            auto r = pool->doGet("/", 300);
            AUTOLI_LOG_INFO(g_logger) << r->toString();
    }, true);
}

void run() {
    autoli::Address::ptr addr = autoli::Address::LookupAnyIPAddress("www.sylar.top:80");
    if(!addr) {
        AUTOLI_LOG_INFO(g_logger) << "get addr error";
        return;
    }

    autoli::Socket::ptr sock = autoli::Socket::CreateTCP(addr);
    bool rt = sock->connect(addr);
    if(!rt) {
        AUTOLI_LOG_INFO(g_logger) << "connect " << *addr << " failed";
        return;
    }

    autoli::http::HttpConnection::ptr conn(new autoli::http::HttpConnection(sock));
    autoli::http::HttpRequest::ptr req(new autoli::http::HttpRequest);
    req->setPath("/blog/");
    req->setHeader("host", "www.sylar.top");
    AUTOLI_LOG_INFO(g_logger) << "req:" << std::endl
        << *req;

    conn->sendRequest(req);
    auto rsp = conn->recvResponse();

    if(!rsp) {
        AUTOLI_LOG_INFO(g_logger) << "recv response error";
        return;
    }
    AUTOLI_LOG_INFO(g_logger) << "rsp:" << std::endl
        << *rsp;

    std::ofstream ofs("rsp.dat");
    ofs << *rsp;

    AUTOLI_LOG_INFO(g_logger) << "=========================";

    auto r = autoli::http::HttpConnection::DoGet("http://www.sylar.top/blog/", 300);//另一种方法
    AUTOLI_LOG_INFO(g_logger) << "result=" << r->result
        << " error=" << r->error
        << " rsp=" << (r->response ? r->response->toString() : "");

    AUTOLI_LOG_INFO(g_logger) << "=========================";
    test_pool();
}

void test_https() {
    auto r = autoli::http::HttpConnection::DoGet("http://www.baidu.com/", 300, {
                        {"Accept-Encoding", "gzip, deflate, br"},
                        {"Connection", "keep-alive"},
                        {"User-Agent", "curl/7.29.0"}
            });
    AUTOLI_LOG_INFO(g_logger) << "result=" << r->result
        << " error=" << r->error
        << " rsp=" << (r->response ? r->response->toString() : "");

    //autoli::http::HttpConnectionPool::ptr pool(new autoli::http::HttpConnectionPool(
    //            "www.baidu.com", "", 80, false, 10, 1000 * 30, 5));
    auto pool = autoli::http::HttpConnectionPool::Create(
                    "https://www.baidu.com", "", 10, 1000 * 30, 5);
    autoli::IOManager::GetThis()->addTimer(1000, [pool](){
            auto r = pool->doGet("/", 3000, {
                        {"Accept-Encoding", "gzip, deflate, br"},
                        {"User-Agent", "curl/7.29.0"}
                    });
            AUTOLI_LOG_INFO(g_logger) << r->toString();
    }, true);
}

void test_data() {
    autoli::Address::ptr addr = autoli::Address::LookupAny("www.baidu.com:80");
    auto sock = autoli::Socket::CreateTCP(addr);

    sock->connect(addr);
    const char buff[] = "GET / HTTP/1.1\r\n"
                "connection: close\r\n"
                "Accept-Encoding: gzip, deflate, br\r\n"
                "Host: www.baidu.com\r\n\r\n";
    sock->send(buff, sizeof(buff));

    std::string line;
    line.resize(1024);

    std::ofstream ofs("http.dat", std::ios::binary);
    int total = 0;
    int len = 0;
    while((len = sock->recv(&line[0], line.size())) > 0) {
        total += len;
        ofs.write(line.c_str(), len);
    }
    std::cout << "total: " << total << " tellp=" << ofs.tellp() << std::endl;
    ofs.flush();
}

void test_parser() {
    std::ifstream ifs("http.dat", std::ios::binary);
    std::string content;
    std::string line;
    line.resize(1024);

    int total = 0;
    while(!ifs.eof()) {
        ifs.read(&line[0], line.size());
        content.append(&line[0], ifs.gcount());
        total += ifs.gcount();
    }

    std::cout << "length: " << content.size() << " total: " << total << std::endl;
    autoli::http::HttpResponseParser parser;
    size_t nparse = parser.execute(&content[0], content.size(), false);
    std::cout << "finish: " << parser.isFinished() << std::endl;
    content.resize(content.size() - nparse);
    std::cout << "rsp: " << *parser.getData() << std::endl;

    auto& client_parser = parser.getParser();
    std::string body;
    int cl = 0;
    do {
        size_t nparse = parser.execute(&content[0], content.size(), true);
        std::cout << "content_len: " << client_parser.content_len
                  << " left: " << content.size()
                  << std::endl;
        cl += client_parser.content_len;
        content.resize(content.size() - nparse);
        body.append(content.c_str(), client_parser.content_len);
        content = content.substr(client_parser.content_len + 2);
    } while(!client_parser.chunks_done);

    std::cout << "total: " << body.size() << " content:" << cl << std::endl;

    autoli::ZlibStream::ptr stream = autoli::ZlibStream::CreateGzip(false);
    stream->write(body.c_str(), body.size());
    stream->flush();

    body = stream->getResult();

    std::ofstream ofs("http.txt");
    ofs << body;
}

int main(int argc, char** argv) {
    autoli::IOManager iom(2);
    iom.schedule(run);
    //iom.schedule(test_https);
    return 0;
}
