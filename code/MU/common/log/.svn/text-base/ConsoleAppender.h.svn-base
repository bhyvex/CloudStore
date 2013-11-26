/*
 * =====================================================================================
 *
 *       Filename:  ConsoleAppender.h
 *
 *    Description:  日志输出器，输出到控制台（输出到标准错误输出）
 *
 *        Version:  1.0
 *        Created:  04/24/2011 02:29:31 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Shi Wei (sw), shiwei2012@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef COMMON_LOG_CONSOLEAPPENDER_H_
#define COMMON_LOG_CONSOLEAPPENDER_H_

#include <cstdarg>
#include <cstddef>

#include <string>

#include "Appender.h"
#include "LogLevel.h"

/**
 * @brief 控制台日志输出器（输出到标准错误输出）
 */
class ConsoleAppender : public Appender {
    public:
        ConsoleAppender() {}
        ~ConsoleAppender() {}

        void log(
                std::string loggerName, 
                LogLevel level, 
                const char *fmt, 
                va_list ap,
                const char *file = NULL,
                int line = 0,
                const char *function = NULL);
};

#endif  // COMMON_LOG_CONSOLEAPPENDER_H_

