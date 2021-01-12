#include "../autoli/socket.h"
#include "../autoli/log.h"
#include "../autoli/iomanager.h"

static autoli::Logger::ptr g_logger = AUTOLI_LOG_ROOT();

void run() {
    autoli::IPAddress::ptr addr = autoli::Address::LookupAnyIPAddress("0.0.0.0:8050");
    autoli::Socket::ptr sock = autoli::Socket::CreateUDP(addr);
    if(sock->bind(addr)) {
        AUTOLI_LOG_INFO(g_logger) << "udp bind : " << *addr;
    } else {
        AUTOLI_LOG_ERROR(g_logger) << "udp bind : " << *addr << " fail";
        return;
    }
    while(true) {
        char buff[1024];
        autoli::Address::ptr from(new autoli::IPv4Address);
        int len = sock->recvFrom(buff, 1024, from);
        if(len > 0) {
            buff[len] = '\0';
            AUTOLI_LOG_INFO(g_logger) << "recv: " << buff << " from: " << *from;
            len = sock->sendTo(buff, len, from);
            if(len < 0) {
                AUTOLI_LOG_INFO(g_logger) << "send: " << buff << " to: " << *from
                    << " error=" << len;
            }
        }
    }
}

int main(int argc, char** argv) {
    autoli::IOManager iom(1);
    iom.schedule(run);
    return 0;
}
