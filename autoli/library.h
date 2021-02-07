#ifndef __AUTOLI_LIBRARY_H__
#define __AUTOLI_LIBRARY_H__

#include <memory>
#include "module.h"

namespace autoli {

class Library {
public:
    static Module::ptr GetModule(const std::string& path);
};

}

#endif
