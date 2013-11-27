/*
 * =====================================================================================
 *
 *       Filename:  Appender.h
 *
 *    Description:  日志输出器接口
 *
 *        Version:  1.0
 *        Created:  04/24/2011 12:02:05 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lpc, lvpengcheng6300@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */


#ifndef COMMON_LOG_APPENDER_H_
#define COMMON_LOG_APPENDER_H_

#include <cstddef>
#include <cstdarg>

#include <string>

#include "LogLevel.h"
#include "sys/noncopyable.h"

/**
 * @brief 日志输出器接口类
 */
class Appender : public noncopyable {
    public:
        /**
         * @brief 析构函数
         */
        virtual ~Appender() {};

        /**
         * @brief 日志输出器初始化
         *
         * @return 
         *      - 0 初始化成功
         *      - -1 初始化失败
         */
        virtual int init() {
            return 0;
        }

        /**
         * @brief 记录日志
         *
         * @param loggerName 日志类名称
         * @param level 日志级别
         * @param fmt 日志格式
         * @param ap 日志参数
         * @param file 产生日志的源文件（仅用于DEBUG）
         * @param line 产生日志的源码行号（仅用于DEBUG）
         * @param function 产生日志的源码所在函数名（仅用于DEBUG）
         */
        virtual void log(
                std::string loggerName, 
                LogLevel level, 
                const char *fmt, 
                va_list ap,
                const char *file = NULL,
                int line = 0,
                const char *function = NULL) = 0;

        /**
         * @brief 日志输出器销毁
         */
        virtual void destroy() {}

    protected:
        /**
         * @brief 获取当前时间，格式为2011-04-24 14:55:35
         *
         * @return 时间字串
         */
        std::string getTimeNow();

        /**
         * @brief 获取进程号的字符串表示
         *
         * @return 
         */
        std::string getPID();

        /**
         * @brief 获取线程号的字符串表示
         *
         * @return 
         */
        std::string getTID();

        /**
         * @brief 获取进程号和线程号的字符串表示
         *
         * @return 
         */
        std::string getPIDAndTID();

        /**
         * @brief 获取日志级别对应的日志级别字符串表示
         *
         * @return 日志级别字串
         */
        std::string getLogLevelString(LogLevel level);
};

#endif  // COMMON_LOG_APPENDER_H_

