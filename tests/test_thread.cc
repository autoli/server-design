#include "../autoli/autoli.h"
#include <unistd.h>

autoli::Logger::ptr g_logger = AUTOLI_LOG_ROOT();

int count = 0;
autoli::RWMutex s_mutex;
//autoli::Mutex s_mutex;

void fun1() {
    AUTOLI_LOG_INFO(g_logger) << "name: " << autoli::Thread::GetName()
                             << " this.name: " << autoli::Thread::GetThis()->getName()
                             << " id: " << autoli::GetThreadId()
                             << " this.id: " << autoli::Thread::GetThis()->getId();

    for(int i = 0; i < 1000000; ++i) {
        autoli::RWMutex::WriteLock lock(s_mutex);
        //autoli::Mutex::Lock lock(s_mutex);
        ++count;
    }
}

void fun2() {
    while(true) {
        AUTOLI_LOG_INFO(g_logger) << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
    }
}

void fun3() {
    while(true) {
        AUTOLI_LOG_INFO(g_logger) << "========================================";
    }
}

int main(int argc, char** argv) {
    AUTOLI_LOG_INFO(g_logger) << "thread test begin";
    //YAML::Node root = YAML::LoadFile("/media/llaron/7E96ADB696AD6F79/代码/高性能服务器/autoli-server/bin/conf/log2.yml");
    //autoli::Config::LoadFromYaml(root);
    
    std::vector<autoli::Thread::ptr> thrs;
    for(int i = 0; i < 5; ++i) {
        autoli::Thread::ptr thr(new autoli::Thread(&fun1, "name_" + std::to_string(i*2)));
        //autoli::Thread::ptr thr2(new autoli::Thread(&fun3, "name_" + std::to_string(i * 2 + 1)));
        thrs.push_back(thr);
        //thrs.push_back(thr2);
    }
    
    for(size_t i = 0; i < thrs.size(); ++i) {
        thrs[i]->join();
    }
    
    AUTOLI_LOG_INFO(g_logger) << "thread test end";
    AUTOLI_LOG_INFO(g_logger) << "count=" << count;
    return 0;
}
