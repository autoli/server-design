#include "autoli/autoli.h"
#include "autoli/ds/hash_multimap.h"

static autoli::Logger::ptr g_logger = AUTOLI_LOG_ROOT();

struct PidVid {
    PidVid(uint32_t p = 0, uint32_t v = 0)
        :pid(p), vid(v) {}
    uint32_t pid;
    uint32_t vid;

    bool operator<(const PidVid& o) const {
        return memcmp(this, &o, sizeof(o)) < 0;
    }
};

void gen() {
    autoli::ds::HashMultimap<int, PidVid> tmp;
    for(int i = 0; i < 500000; ++i) {
        int32_t len = rand() % 10 + 5;
        int k = rand();
        for(int n = 0; n < len; ++n) {
            tmp.insert(k, PidVid(rand(), rand()));
        }
    }

    std::ofstream ofs("./hashmultimap.data");
    tmp.writeTo(ofs);
}

void test() {
    for(int i = 0; i < 10000; ++i) {
        AUTOLI_LOG_INFO(g_logger) << "i=" << i;
        std::ifstream ifs("./hashmultimap.data");
        autoli::ds::HashMultimap<int, PidVid> tmp;
        if(!tmp.readFrom(ifs)) {
            AUTOLI_LOG_INFO(g_logger) << "error";
        }
        if(i % 100 == 0) {
            AUTOLI_LOG_INFO(g_logger) << "over..." << (i + 1);
        }
    }
}

int main(int argc, char** argv) {
    gen();
    test();
    return 0;
}
