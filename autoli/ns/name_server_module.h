#ifndef __AUTOLI_NS_NAME_SERVER_MODULE_H__
#define __AUTOLI_NS_NAME_SERVER_MODULE_H__

#include "autoli/module.h"
#include "ns_protocol.h"

namespace autoli {
namespace ns {

class NameServerModule;
class NSClientInfo {
friend class NameServerModule;
public:
    typedef std::shared_ptr<NSClientInfo> ptr;
private:
    NSNode::ptr m_node;
    std::map<std::string, std::set<uint32_t> > m_domain2cmds;
};

class NameServerModule : public RockModule {
public:
    typedef std::shared_ptr<NameServerModule> ptr;
    NameServerModule();

    virtual bool handleRockRequest(autoli::RockRequest::ptr request
                        ,autoli::RockResponse::ptr response
                        ,autoli::RockStream::ptr stream) override;
    virtual bool handleRockNotify(autoli::RockNotify::ptr notify
                        ,autoli::RockStream::ptr stream) override;
    virtual bool onConnect(autoli::Stream::ptr stream) override;
    virtual bool onDisconnect(autoli::Stream::ptr stream) override;
    virtual std::string statusString() override;
private:
    bool handleRegister(autoli::RockRequest::ptr request
                        ,autoli::RockResponse::ptr response
                        ,autoli::RockStream::ptr stream);
    bool handleQuery(autoli::RockRequest::ptr request
                        ,autoli::RockResponse::ptr response
                        ,autoli::RockStream::ptr stream);
    bool handleTick(autoli::RockRequest::ptr request
                        ,autoli::RockResponse::ptr response
                        ,autoli::RockStream::ptr stream);

private:
    NSClientInfo::ptr get(autoli::RockStream::ptr rs);
    void set(autoli::RockStream::ptr rs, NSClientInfo::ptr info);

    void setQueryDomain(autoli::RockStream::ptr rs, const std::set<std::string>& ds);

    void doNotify(std::set<std::string>& domains, std::shared_ptr<NotifyMessage> nty);

    std::set<autoli::RockStream::ptr> getStreams(const std::string& domain);
private:
    NSDomainSet::ptr m_domains;

    autoli::RWMutex m_mutex;
    std::map<autoli::RockStream::ptr, NSClientInfo::ptr> m_sessions;

    /// sessoin 关注的域名
    std::map<autoli::RockStream::ptr, std::set<std::string> > m_queryDomains;
    /// 域名对应关注的session
    std::map<std::string, std::set<autoli::RockStream::ptr> > m_domainToSessions;
};

}
}

#endif
