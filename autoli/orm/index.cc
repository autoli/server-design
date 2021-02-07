#include "index.h"
#include "autoli/log.h"
#include "autoli/util.h"

namespace autoli {
namespace orm {

static autoli::Logger::ptr g_logger = AUTOLI_LOG_NAME("orm");

Index::Type Index::ParseType(const std::string& v) {
#define XX(a, b) \
    if(v == b) {\
        return a; \
    }
    XX(TYPE_PK, "pk");
    XX(TYPE_UNIQ, "uniq");
    XX(TYPE_INDEX, "index");
#undef XX
    return TYPE_NULL;
}

std::string Index::TypeToString(Type v) {
#define XX(a, b) \
    if(v == a) { \
        return b; \
    }
    XX(TYPE_PK, "pk");
    XX(TYPE_UNIQ, "uniq");
    XX(TYPE_INDEX, "index");
#undef XX
    return "";
}

bool Index::init(const tinyxml2::XMLElement& node) {
    if(!node.Attribute("name")) {
        AUTOLI_LOG_ERROR(g_logger) << "index name not exists";
        return false;
    }
    m_name = node.Attribute("name");

    if(!node.Attribute("type")) {
        AUTOLI_LOG_ERROR(g_logger) << "index name=" << m_name << " type is null";
        return false;
    }

    m_type = node.Attribute("type");
    m_dtype = ParseType(m_type);
    if(m_dtype == TYPE_NULL) {
        AUTOLI_LOG_ERROR(g_logger) << "index name=" << m_name << " type=" << m_type
            << " invalid (pk, index, uniq)";
        return false;
    }

    if(!node.Attribute("cols")) {
        AUTOLI_LOG_ERROR(g_logger) << "index name=" << m_name << " cols is null";
    }
    std::string tmp = node.Attribute("cols");
    m_cols = autoli::split(tmp, ',');

    if(node.Attribute("desc")) {
        m_desc = node.Attribute("desc");
    }
    return true;
}

}
}
