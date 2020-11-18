//
// Created by llaron on 2020/11/15.
//

#include <iostream>
#include "../autoli/log.h"

int main(int argc, char** argv) {
//    autoli::Logger::ptr logger(new autoli::Logger);
//    logger->addAppender(autoli::LogAppender::ptr(new autoli::StdoutLogAppender));
//
//    autoli::FileLogAppender::ptr file_appender(new autoli::FileLogAppender("./log.txt"));
//    autoli::LogFormatter::ptr fmt(new autoli::LogFormatter("%d%T%p%T%m%n"));
//    file_appender->setFormatter(fmt);
//    file_appender->setLevel(autoli::LogLevel::ERROR);
//
//    logger->addAppender(file_appender);

    //sylar::LogEvent::ptr event(new sylar::LogEvent(__FILE__, __LINE__, 0, sylar::GetThreadId(), sylar::GetFiberId(), time(0)));
    //event->getSS() << "hello sylar log";
    //logger->log(sylar::LogLevel::DEBUG, event);
    std::cout << "hello autoli log" << std::endl;
//
//    SYLAR_LOG_INFO(logger) << "test macro";
//    SYLAR_LOG_ERROR(logger) << "test macro error";
//
//    SYLAR_LOG_FMT_ERROR(logger, "test macro fmt error %s", "aa");
//
//    auto l = autoli::LoggerMgr::GetInstance()->getLogger("xx");
//    SYLAR_LOG_INFO(l) << "xxx";
    return 0;
}