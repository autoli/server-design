#include "../autoli/socket.h"
#include "../autoli/autoli.h"
#include "../autoli/iomanager.h"

static autoli::Logger::ptr g_looger = AUTOLI_LOG_ROOT();

void test_socket() {
    //std::vector<autoli::Address::ptr> addrs;
    //autoli::Address::Lookup(addrs, "www.baidu.com", AF_INET);
    //autoli::IPAddress::ptr addr;
    //for(auto& i : addrs) {
    //    AUTOLI_LOG_INFO(g_looger) << i->toString();
    //    addr = std::dynamic_pointer_cast<autoli::IPAddress>(i);
    //    if(addr) {
    //        break;
    //    }
    //}
    autoli::IPAddress::ptr addr = autoli::Address::LookupAnyIPAddress("www.baidu.com");
    if(addr) {
        AUTOLI_LOG_INFO(g_looger) << "get address: " << addr->toString();
    } else {
        AUTOLI_LOG_ERROR(g_looger) << "get address fail";
        return;
    }

    autoli::Socket::ptr sock = autoli::Socket::CreateTCP(addr);
    addr->setPort(80);
    AUTOLI_LOG_INFO(g_looger) << "addr=" << addr->toString();
    if(!sock->connect(addr)) {
        AUTOLI_LOG_ERROR(g_looger) << "connect " << addr->toString() << " fail";
        return;
    } else {
        AUTOLI_LOG_INFO(g_looger) << "connect " << addr->toString() << " connected";
    }

    const char buff[] = "GET / HTTP/1.0\r\n\r\n";
    int rt = sock->send(buff, sizeof(buff));
    if(rt <= 0) {
        AUTOLI_LOG_INFO(g_looger) << "send fail rt=" << rt;
        return;
    }

    std::string buffs;
    buffs.resize(4096);
    rt = sock->recv(&buffs[0], buffs.size());

    if(rt <= 0) {
        AUTOLI_LOG_INFO(g_looger) << "recv fail rt=" << rt;
        return;
    }

    buffs.resize(rt);
    AUTOLI_LOG_INFO(g_looger) << buffs;
}

void test2() {
    autoli::IPAddress::ptr addr = autoli::Address::LookupAnyIPAddress("www.baidu.com:80");
    if(addr) {
        AUTOLI_LOG_INFO(g_looger) << "get address: " << addr->toString();
    } else {
        AUTOLI_LOG_ERROR(g_looger) << "get address fail";
        return;
    }

    autoli::Socket::ptr sock = autoli::Socket::CreateTCP(addr);
    if(!sock->connect(addr)) {
        AUTOLI_LOG_ERROR(g_looger) << "connect " << addr->toString() << " fail";
        return;
    } else {
        AUTOLI_LOG_INFO(g_looger) << "connect " << addr->toString() << " connected";
    }

    uint64_t ts = autoli::GetCurrentUS();
    for(size_t i = 0; i < 10000000000ul; ++i) {
        if(int err = sock->getError()) {
            AUTOLI_LOG_INFO(g_looger) << "err=" << err << " errstr=" << strerror(err);
            break;
        }

        //struct tcp_info tcp_info;
        //if(!sock->getOption(IPPROTO_TCP, TCP_INFO, tcp_info)) {
        //    AUTOLI_LOG_INFO(g_looger) << "err";
        //    break;
        //}
        //if(tcp_info.tcpi_state != TCP_ESTABLISHED) {
        //    AUTOLI_LOG_INFO(g_looger)
        //            << " state=" << (int)tcp_info.tcpi_state;
        //    break;
        //}
        static int batch = 10000000;
        if(i && (i % batch) == 0) {
            uint64_t ts2 = autoli::GetCurrentUS();
            AUTOLI_LOG_INFO(g_looger) << "i=" << i << " used: " << ((ts2 - ts) * 1.0 / batch) << " us";
            ts = ts2;
        }
    }
}

int main(int argc, char** argv) {
    autoli::IOManager iom;
    iom.schedule(&test_socket);
    //iom.schedule(&test2);
    return 0;
}
