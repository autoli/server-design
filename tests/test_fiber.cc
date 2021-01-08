#include "../autoli/autoli.h"

autoli::Logger::ptr g_logger = AUTOLI_LOG_ROOT();

void run_in_fiber() {
    AUTOLI_LOG_INFO(g_logger) << "run_in_fiber begin";
    autoli::Fiber::YieldToHold();
    AUTOLI_LOG_INFO(g_logger) << "run_in_fiber end";
    autoli::Fiber::YieldToHold();
}

void test_fiber() {
    AUTOLI_LOG_INFO(g_logger) << "main begin -1";
    {
        autoli::Fiber::GetThis();
        AUTOLI_LOG_INFO(g_logger) << "main begin";
        autoli::Fiber::ptr fiber(new autoli::Fiber(run_in_fiber));
        fiber->swapIn();
        AUTOLI_LOG_INFO(g_logger) << "main after swapIn";
        fiber->swapIn();
        AUTOLI_LOG_INFO(g_logger) << "main after end";
        fiber->swapIn();
    }
    AUTOLI_LOG_INFO(g_logger) << "main after end2";
}

int main(int argc, char** argv) {
    autoli::Thread::SetName("main");
    std::vector<autoli::Thread::ptr> thrs;
    for(int i = 0; i < 3; ++i) {
        thrs.push_back(autoli::Thread::ptr(
                    new autoli::Thread(&test_fiber, "name_" + std::to_string(i))));
    }
    for(auto i : thrs) {
        i->join();
    }
    return 0;
}
