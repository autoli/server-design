//
// Created by llaron on 2020/12/24.
//

#ifndef __AUTOLI_UTIL_H__
#define __AUTOLI_UTIL_H__

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

}
#endif
