#include "../autoli/tcp_server.h"
#include "../autoli/iomanager.h"
#include "../autoli/log.h"

autoli::Logger::ptr g_logger = AUTOLI_LOG_ROOT();

void run() {
    auto addr = autoli::Address::LookupAny("0.0.0.0:8033");
    //auto addr2 = autoli::UnixAddress::ptr(new autoli::UnixAddress("/tmp/unix_addr"));
    //AUTOLI_LOG_INFO(g_logger)<<*addr<<"-"<<*addr2;
    std::vector<autoli::Address::ptr> addrs;
    addrs.push_back(addr);
    //addrs.push_back(addr2);

    autoli::TcpServer::ptr tcp_server(new autoli::TcpServer);
    std::vector<autoli::Address::ptr> fails;
    while(!tcp_server->bind(addrs, fails)) {
        sleep(2);
    }
    tcp_server->start();
    
}
int main(int argc, char** argv) {
    autoli::IOManager iom(2);
    iom.schedule(run);
    return 0;
    //ls -lh /tmp/unix_addr
    //rm /tmp/unix_addr
    //telnet 127.0.0.1 8033
    //netstat -ap | grep test_
    //netstat -tnalp | grep test_

}
