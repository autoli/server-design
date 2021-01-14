#ifndef __AUTOLI_HTTP_SERVLETS_STATUS_SERVLET_H__
#define __AUTOLI_HTTP_SERVLETS_STATUS_SERVLET_H__

#include "autoli/http/servlet.h"

namespace autoli {
namespace http {

class StatusServlet : public Servlet {
public:
    StatusServlet();
    virtual int32_t handle(autoli::http::HttpRequest::ptr request
                   , autoli::http::HttpResponse::ptr response
                   , autoli::http::HttpSession::ptr session) override;
};

}
}

#endif
