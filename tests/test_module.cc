#include "autoli/module.h"
#include "autoli/singleton.h"
#include <iostream>
#include "autoli/log.h"
#include "autoli/db/redis.h"

static autoli::Logger::ptr g_logger = AUTOLI_LOG_ROOT();

class A {
public:
    A() {
        std::cout << "A::A " << this << std::endl;
    }

    ~A() {
        std::cout << "A::~A " << this << std::endl;
    }

};

class MyModule : public autoli::RockModule {
public:
    MyModule()
        :RockModule("hello", "1.0", "") {
        //autoli::Singleton<A>::GetInstance();
    }

    bool onLoad() override {
        autoli::Singleton<A>::GetInstance();
        std::cout << "-----------onLoad------------" << std::endl;
        return true;
    }

    bool onUnload() override {
        autoli::Singleton<A>::GetInstance();
        std::cout << "-----------onUnload------------" << std::endl;
        return true;
    }

    bool onServerReady() {
        registerService("rock", "autoli.top", "blog");
        auto rpy = autoli::RedisUtil::Cmd("local", "get abc");
        if(!rpy) {
            AUTOLI_LOG_ERROR(g_logger) << "redis cmd get abc error";
        } else {
            AUTOLI_LOG_ERROR(g_logger) << "redis get abc: "
                << (rpy->str ? rpy->str : "(null)");
        }
        return true;
    }

    bool handleRockRequest(autoli::RockRequest::ptr request
                        ,autoli::RockResponse::ptr response
                        ,autoli::RockStream::ptr stream) {
        //AUTOLI_LOG_INFO(g_logger) << "handleRockRequest " << request->toString();
        //sleep(1);
        response->setResult(0);
        response->setResultStr("ok");
        response->setBody("echo: " + request->getBody());

        usleep(100 * 1000);
        auto addr = stream->getLocalAddressString();
        if(addr.find("8061") != std::string::npos) {
            if(rand() % 100 < 50) {
                usleep(10 * 1000);
            } else if(rand() % 100 < 10) {
                response->setResult(-1000);
            }
        } else {
            //if(rand() % 100 < 25) {
            //    usleep(10 * 1000);
            //} else if(rand() % 100 < 10) {
            //    response->setResult(-1000);
            //}
        }
        return true;
        //return rand() % 100 < 90;
    }

    bool handleRockNotify(autoli::RockNotify::ptr notify 
                        ,autoli::RockStream::ptr stream) {
        AUTOLI_LOG_INFO(g_logger) << "handleRockNotify " << notify->toString();
        return true;
    }

};

extern "C" {

autoli::Module* CreateModule() {
    autoli::Singleton<A>::GetInstance();
    std::cout << "=============CreateModule=================" << std::endl;
    return new MyModule;
}

void DestoryModule(autoli::Module* ptr) {
    std::cout << "=============DestoryModule=================" << std::endl;
    delete ptr;
}

}
