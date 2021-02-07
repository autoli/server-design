#ifndef __AUTOLI_ROCK_SERVER_H__
#define __AUTOLI_ROCK_SERVER_H__

#include "autoli/rock/rock_stream.h"
#include "autoli/tcp_server.h"

namespace autoli {

class RockServer : public TcpServer {
public:
    typedef std::shared_ptr<RockServer> ptr;
    RockServer(const std::string& type = "rock"
               ,autoli::IOManager* worker = autoli::IOManager::GetThis()
               ,autoli::IOManager* io_worker = autoli::IOManager::GetThis()
               ,autoli::IOManager* accept_worker = autoli::IOManager::GetThis());

protected:
    virtual void handleClient(Socket::ptr client) override;
};

}

#endif
