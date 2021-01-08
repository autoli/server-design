#include "../autoli/autoli.h"
#include "../autoli/iomanager.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <sys/epoll.h>

autoli::Logger::ptr g_logger = AUTOLI_LOG_ROOT();

int sock = 0;

void test_fiber() {
    AUTOLI_LOG_INFO(g_logger) << "test_fiber sock=" << sock;
    
    //sleep(3);

    //close(sock);
    //autoli::IOManager::GetThis()->cancelAll(sock);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    fcntl(sock, F_SETFL, O_NONBLOCK);

    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(80);
    inet_pton(AF_INET, "14.215.177.38", &addr.sin_addr.s_addr);
    
    if(!connect(sock, (const sockaddr*)&addr, sizeof(addr))) {
    } else if(errno == EINPROGRESS) {
        AUTOLI_LOG_INFO(g_logger) << "add event errno=" << errno << " " << strerror(errno);
        autoli::IOManager::GetThis()->addEvent(sock, autoli::IOManager::READ, [](){
            AUTOLI_LOG_INFO(g_logger) << "read callback";
        });
        autoli::IOManager::GetThis()->addEvent(sock, autoli::IOManager::WRITE, [](){
            AUTOLI_LOG_INFO(g_logger) << "write callback";
            //close(sock);
            autoli::IOManager::GetThis()->cancelEvent(sock, autoli::IOManager::READ);
            close(sock);
        });
    } else {
        AUTOLI_LOG_INFO(g_logger) << "else " << errno << " " << strerror(errno);
    }

}

void test1() {
    std::cout << "EPOLLIN=" << EPOLLIN
              << " EPOLLOUT=" << EPOLLOUT << std::endl;
    autoli::IOManager iom(2, false);
    iom.schedule(&test_fiber);
}

autoli::Timer::ptr s_timer;
void test_timer() {
    autoli::IOManager iom(2);
    s_timer = iom.addTimer(1000, [](){
        static int i = 0;
        AUTOLI_LOG_INFO(g_logger) << "hello timer i=" << i;
        if(++i == 3) {
            s_timer->reset(2000, true);
            //s_timer->cancel();
        }
    }, true);
}

int main(int argc, char** argv) {
    test1();
    //test_timer();
    return 0;
}
