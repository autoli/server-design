#include "../autoli/daemon.h"
#include "../autoli/iomanager.h"
#include "../autoli/log.h"

static autoli::Logger::ptr g_logger = AUTOLI_LOG_ROOT();

autoli::Timer::ptr timer;
int server_main(int argc, char** argv) {
    AUTOLI_LOG_INFO(g_logger) << autoli::ProcessInfoMgr::GetInstance()->toString();
    autoli::IOManager iom(1);
    timer = iom.addTimer(1000, [](){
            AUTOLI_LOG_INFO(g_logger) << "onTimer";
            static int count = 0;
            if(++count > 10) {
                exit(1);
            }
    }, true);
    return 0;
}

int main(int argc, char** argv) {
    return autoli::start_daemon(argc, argv, server_main, argc != 1);
}
