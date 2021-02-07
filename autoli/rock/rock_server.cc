#include "rock_server.h"
#include "autoli/log.h"
#include "autoli/module.h"

namespace autoli {

static autoli::Logger::ptr g_logger = AUTOLI_LOG_NAME("system");

RockServer::RockServer(const std::string& type
                       ,autoli::IOManager* worker
                       ,autoli::IOManager* io_worker
                       ,autoli::IOManager* accept_worker)
    :TcpServer(worker, io_worker, accept_worker) {
    m_type = type;
}

void RockServer::handleClient(Socket::ptr client) {
    AUTOLI_LOG_DEBUG(g_logger) << "handleClient " << *client;
    autoli::RockSession::ptr session(new autoli::RockSession(client));
    session->setWorker(m_worker);
    ModuleMgr::GetInstance()->foreach(Module::ROCK,
            [session](Module::ptr m) {
        m->onConnect(session);
    });
    session->setDisconnectCb(
        [](AsyncSocketStream::ptr stream) {
             ModuleMgr::GetInstance()->foreach(Module::ROCK,
                    [stream](Module::ptr m) {
                m->onDisconnect(stream);
            });
        }
    );
    session->setRequestHandler(
        [](autoli::RockRequest::ptr req
           ,autoli::RockResponse::ptr rsp
           ,autoli::RockStream::ptr conn)->bool {
            //AUTOLI_LOG_INFO(g_logger) << "handleReq " << req->toString()
            //                         << " body=" << req->getBody();
            bool rt = false;
            ModuleMgr::GetInstance()->foreach(Module::ROCK,
                    [&rt, req, rsp, conn](Module::ptr m) {
                if(rt) {
                    return;
                }
                rt = m->handleRequest(req, rsp, conn);
            });
            return rt;
        }
    ); 
    session->setNotifyHandler(
        [](autoli::RockNotify::ptr nty
           ,autoli::RockStream::ptr conn)->bool {
            AUTOLI_LOG_INFO(g_logger) << "handleNty " << nty->toString()
                                     << " body=" << nty->getBody();
            bool rt = false;
            ModuleMgr::GetInstance()->foreach(Module::ROCK,
                    [&rt, nty, conn](Module::ptr m) {
                if(rt) {
                    return;
                }
                rt = m->handleNotify(nty, conn);
            });
            return rt;
        }
    );
    session->start();
}

}
