#ifndef __AUTOLI_NS_NS_CLIENT_H__
#define __AUTOLI_NS_NS_CLIENT_H__

#include "autoli/rock/rock_stream.h"
#include "ns_protocol.h"

namespace autoli {
namespace ns {

class NSClient : public RockConnection {
public:
    typedef std::shared_ptr<NSClient> ptr;
    NSClient();
    ~NSClient();

    const std::set<std::string>& getQueryDomains();
    void setQueryDomains(const std::set<std::string>& v);

    void addQueryDomain(const std::string& domain);
    void delQueryDomain(const std::string& domain);

    bool hasQueryDomain(const std::string& domain);

    RockResult::ptr query();

    void init();
    void uninit();
    NSDomainSet::ptr getDomains() const { return m_domains;}
private:
    void onQueryDomainChange();
    bool onConnect(autoli::AsyncSocketStream::ptr stream);
    void onDisconnect(autoli::AsyncSocketStream::ptr stream);
    bool onNotify(autoli::RockNotify::ptr ,autoli::RockStream::ptr);

    void onTimer();
private:
    autoli::RWMutex m_mutex;
    std::set<std::string> m_queryDomains;
    NSDomainSet::ptr m_domains;
    uint32_t m_sn = 0;
    autoli::Timer::ptr m_timer;
};

}
}

#endif
