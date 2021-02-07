#include "autoli/env.h"
#include <unistd.h>
#include <iostream>
#include <fstream>

struct A {
    A() {
        std::ifstream ifs("/proc/" + std::to_string(getpid()) + "/cmdline", std::ios::binary);
        std::string content;
        content.resize(4096);

        ifs.read(&content[0], content.size());
        content.resize(ifs.gcount());

        for(size_t i = 0; i < content.size(); ++i) {
            std::cout << i << " - " << content[i] << " - " << (int)content[i] << std::endl;
        }
    }
};

A a;

int main(int argc, char** argv) {
    std::cout << "argc=" << argc << std::endl;
    autoli::EnvMgr::GetInstance()->addHelp("s", "start with the terminal");
    autoli::EnvMgr::GetInstance()->addHelp("d", "run as daemon");
    autoli::EnvMgr::GetInstance()->addHelp("p", "print help");
    if(!autoli::EnvMgr::GetInstance()->init(argc, argv)) {
        autoli::EnvMgr::GetInstance()->printHelp();
        return 0;
    }

    std::cout << "exe=" << autoli::EnvMgr::GetInstance()->getExe() << std::endl;
    std::cout << "cwd=" << autoli::EnvMgr::GetInstance()->getCwd() << std::endl;

    std::cout << "path=" << autoli::EnvMgr::GetInstance()->getEnv("PATH", "xxx") << std::endl;
    std::cout << "test=" << autoli::EnvMgr::GetInstance()->getEnv("TEST", "") << std::endl;
    std::cout << "set env " << autoli::EnvMgr::GetInstance()->setEnv("TEST", "yy") << std::endl;
    std::cout << "test=" << autoli::EnvMgr::GetInstance()->getEnv("TEST", "") << std::endl;
    if(autoli::EnvMgr::GetInstance()->has("p")) {
        autoli::EnvMgr::GetInstance()->printHelp();
    }
    return 0;
}
