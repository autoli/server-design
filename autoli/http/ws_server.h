#ifndef __AUTOLI_HTTP_WS_SERVER_H__
#define __AUTOLI_HTTP_WS_SERVER_H__

#include "autoli/tcp_server.h"
#include "ws_session.h"
#include "ws_servlet.h"

namespace autoli {
namespace http {

class WSServer : public TcpServer {
public:
    typedef std::shared_ptr<WSServer> ptr;

    WSServer(autoli::IOManager* worker = autoli::IOManager::GetThis()
             , autoli::IOManager* io_worker = autoli::IOManager::GetThis()
             , autoli::IOManager* accept_worker = autoli::IOManager::GetThis());

    WSServletDispatch::ptr getWSServletDispatch() const { return m_dispatch;}
    void setWSServletDispatch(WSServletDispatch::ptr v) { m_dispatch = v;}
protected:
    virtual void handleClient(Socket::ptr client) override;
protected:
    WSServletDispatch::ptr m_dispatch;
};

}
}

#endif
