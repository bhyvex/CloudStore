/*
 * =====================================================================================
 *
 *       Filename:  Logger.h
 *
 *    Description:  日志类
 *
 *        Version:  1.0
 *        Created:  04/24/2011 12:37:53 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Shi Wei (sw), shiwei2012@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef COMMON_LOG_LOGGER_H_
#define COMMON_LOG_LOGGER_H_

#include <cstddef>
#include <cstdarg>

#include <list>
#include <string>

#include "sys/noncopyable.h"
#include "LogLevel.h"

class Appender;
class LoggerFactory;

/**
 * @brief 日志类定义
 */
class Logger : public noncopyable {
    friend class LoggerFactory;
    public:
        /**
         * @brief 严重错误
         *
         * @param fmt 日志格式
         * @param ...
         */
        void fatal(const char *fmt, ...);

        /**
         * @brief 错误
         *
         * @param fmt 日志格式
         * @param ...
         */
        void error(const char *fmt, ...);

        /**
         * @brief 警告
         *
         * @param fmt 日志格式
         * @param ...
         */
        void warn(const char *fmt, ...);

        /**
         * @brief 普通信息
         *
         * @param fmt 日志格式
         * @param ...
         */
        void info(const char *fmt, ...);

        /**
         * @brief 调试信息
         *
         * @param file 产生日志的源文件
         * @param line 产生日志的源码行号
         * @param function 产生日志的源码行所在函数名称
         * @param fmt 日志格式
         * @param ... 日志参数
         */
        void debug(
                const char *file,
                int line,
                const char *function,
                const char *fmt, ...);

        /**
         * @brief 跟踪信息
         *
         * @param fmt 日志格式
         * @param ...
         */
        void trace(
                const char *file,
                int line,
                const char *function,
                const char *fmt, ...);

    private:
        Logger();  // can't new

        /**
         * @brief 析构函数
         */
        virtual ~Logger();


        /**
         * @brief 设置当前日志级别，低于此级别的日志将不被记录
         *
         * @param level
         */
        void setLevel(LogLevel level);

        /**
         * @brief 添加一个日志输出器
         *
         * @param apdr
         */
        void addAppender(Appender *apdr);

        /**
         * @brief 设置Logger的名称
         *
         * @param name
         */
        void setName(const std::string &name);

        /**
         * @brief 执行具体日志记录操作
         *
         * @param level 日志级别
         * @param fmt 日志格式
         * @param ap 日志参数
         * @param file  产生日志的源文件（仅用于DEBUG)
         * @param line  产生日志的源码行号（仅用于DEBUG）
         * @param function 产生日志的源码行所在函数名称（仅用于DEBUG)
         */
        void doLog(
                LogLevel level, 
                const char *fmt, 
                va_list ap, 
                const char *file = NULL,
                int line = 0,
                const char *function = NULL);

    private:
        LogLevel m_Level;  // log level
        std::list<Appender *> m_AppenderList;  // list of Appenders
        std::string m_Name;  // Logger name
};
#endif  // COMMON_LOG_LOGGER_H_

