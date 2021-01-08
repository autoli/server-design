#include "../autoli/autoli.h"

static autoli::Logger::ptr g_logger = AUTOLI_LOG_ROOT();

void test_fiber() {
    static int s_count = 5;
    AUTOLI_LOG_INFO(g_logger) << "test in fiber s_count=" << s_count;
    sleep(1);
    if(--s_count >= 0) {
        autoli::Scheduler::GetThis()->schedule(&test_fiber, autoli::GetThreadId());
    }
}

int main(int argc, char** argv) {
    AUTOLI_LOG_INFO(g_logger) << "main";
    autoli::Scheduler sc(3, false, "test");
    sc.start();
    sleep(2);
    AUTOLI_LOG_INFO(g_logger) << "schedule";
    sc.schedule(&test_fiber);
    sc.stop();
    AUTOLI_LOG_INFO(g_logger) << "over";
    return 0;
}
