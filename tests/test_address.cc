#include "../autoli/address.h"
#include "../autoli/log.h"

autoli::Logger::ptr g_logger = AUTOLI_LOG_ROOT();

void test() {
    std::vector<autoli::Address::ptr> addrs;

    AUTOLI_LOG_INFO(g_logger) << "begin";
    //bool v = autoli::Address::Lookup(addrs, "localhost:3080");
    //bool v = autoli::Address::Lookup(addrs, "www.baidu.com:ftp", AF_INET);
    bool v = autoli::Address::Lookup(addrs, "www.sylar.top", AF_INET);
    AUTOLI_LOG_INFO(g_logger) << "end";
    if(!v) {
        AUTOLI_LOG_ERROR(g_logger) << "lookup fail";
        return;
    }

    for(size_t i = 0; i < addrs.size(); ++i) {
        AUTOLI_LOG_INFO(g_logger) << i << " - " << addrs[i]->toString();
    }

    auto addr = autoli::Address::LookupAny("localhost:4080");
    if(addr) {
        AUTOLI_LOG_INFO(g_logger) << *addr;
    } else {
        AUTOLI_LOG_ERROR(g_logger) << "error";
    }
}

void test_iface() {
    std::multimap<std::string, std::pair<autoli::Address::ptr, uint32_t> > results;

    bool v = autoli::Address::GetInterfaceAddresses(results);
    if(!v) {
        AUTOLI_LOG_ERROR(g_logger) << "GetInterfaceAddresses fail";
        return;
    }

    for(auto& i: results) {
        AUTOLI_LOG_INFO(g_logger) << i.first << " - " << i.second.first->toString() << " - "
            << i.second.second;
    }
}

void test_ipv4() {
    //auto addr = autoli::IPAddress::Create("www.sylar.top");
    auto addr = autoli::IPAddress::Create("127.0.0.8");
    if(addr) {
        AUTOLI_LOG_INFO(g_logger) << addr->toString();
    }
}

int main(int argc, char** argv) {
    test_ipv4();
    //test_iface();
    //test();
    return 0;
}
