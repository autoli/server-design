/**
 * @file macro.h
 * @brief 常用宏的封装
 * @author autoli.yin
 * @email 564628276@qq.com
 * @date 2019-06-01
 * @copyright Copyright (c) 2019年 autoli.yin All rights reserved (www.autoli.top)
 */
#ifndef __AUTOLI_MACRO_H__
#define __AUTOLI_MACRO_H__

#include <string.h>
#include <assert.h>
#include "log.h"
#include "util.h"

#if defined __GNUC__ || defined __llvm__
/// LIKCLY 宏的封装, 告诉编译器优化,条件大概率成立
#   define AUTOLI_LIKELY(x)       __builtin_expect(!!(x), 1)
/// LIKCLY 宏的封装, 告诉编译器优化,条件大概率不成立
#   define AUTOLI_UNLIKELY(x)     __builtin_expect(!!(x), 0)
#else
#   define AUTOLI_LIKELY(x)      (x)
#   define AUTOLI_UNLIKELY(x)      (x)
#endif

/// 断言宏封装
#define AUTOLI_ASSERT(x) \
    if(AUTOLI_UNLIKELY(!(x))) { \
        AUTOLI_LOG_ERROR(AUTOLI_LOG_ROOT()) << "ASSERTION: " #x \
            << "\nbacktrace:\n" \
            << autoli::BacktraceToString(100, 2, "    "); \
        assert(x); \
    }

/// 断言宏封装
#define AUTOLI_ASSERT2(x, w) \
    if(AUTOLI_UNLIKELY(!(x))) { \
        AUTOLI_LOG_ERROR(AUTOLI_LOG_ROOT()) << "ASSERTION: " #x \
            << "\n" << w \
            << "\nbacktrace:\n" \
            << autoli::BacktraceToString(100, 2, "    "); \
        assert(x); \
    }

#endif
