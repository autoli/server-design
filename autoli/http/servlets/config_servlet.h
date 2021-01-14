#ifndef __AUTOLI_HTTP_SERVLETS_CONFIG_SERVLET_H__
#define __AUTOLI_HTTP_SERVLETS_CONFIG_SERVLET_H__

#include "autoli/http/servlet.h"

namespace autoli {
namespace http {

class ConfigServlet : public Servlet {
public:
    ConfigServlet();
    virtual int32_t handle(autoli::http::HttpRequest::ptr request
                   , autoli::http::HttpResponse::ptr response
                   , autoli::http::HttpSession::ptr session) override;
};

}
}

#endif
