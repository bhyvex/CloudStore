/*
 * =====================================================================================
 *
 *       Filename:  LoggerFactory.h
 *
 *    Description:  日志工厂类
 *
 *        Version:  1.0
 *        Created:  04/24/2011 04:34:00 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lpc, lvpengcheng6300@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef COMMON_LOG_LOGGERFACTORY_H_
#define COMMON_LOG_LOGGERFACTORY_H_

#include <map>
#include <string>

#include "sys/Singleton.h"
#include "Logger.h"
#include "LogLevel.h"

/**
 * @brief 日志工厂类
 */
class LoggerFactory : public Singleton<LoggerFactory> {
    friend class Singleton<LoggerFactory>;
    public:
        ~LoggerFactory();

        /**
         * @brief 初始化日志子系统
         *
         * @param level 日志显示级别
         *
         * @return 
         */
        int initialize(LogLevel level);

        /**
         * @brief 获取指定Logger
         *
         * @param loggerName Logger的名称
         *
         * @return 指向Logger的指针
         *      - NULL 指定的Logger不存在
         */
        Logger* getLogger(const std::string &loggerName);

        /**
         * @brief 获取ROOT Logger（ROOT Logger总是存在）
         *
         * @return 指向ROOT Logger的指针
         */
        Logger* getRootLogger();

    private:
        LoggerFactory();  // can't new

    private:
        LogLevel m_Level;
        static std::map<std::string, Logger *> m_LoggerMap;  // map of logger
};

#endif // COMMON_LOG_LOGGERFACTORY_H_

