//
// Created by llaron on 2020/12/24.
//

#ifndef __AUTOLI_UTIL_H__
#define __AUTOLI_UTIL_H__

#include <cxxabi.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <stdint.h>

namespace autoli{

/**
 * @brief 返回当前线程的ID
 */
    pid_t GetThreadId();

/**
 * @brief 返回当前协程的ID
 */
    uint32_t GetFiberId();//uint32_t需要用stdint.h

    template<class T>
    const char* TypeToName() {
        static const char* s_name = abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, nullptr);
        return s_name;
    }
}
#endif
