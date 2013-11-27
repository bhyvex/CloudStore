/*
 * =====================================================================================
 *
 *       Filename:  ConsoleAppender.cpp
 *
 *    Description:  日志输出器，输出到控制台（输出到标准错误输出）
 *
 *        Version:  1.0
 *        Created:  04/24/2011 02:40:14 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lpc, lvpengcheng6300@gmail.com
 *        Company:
 *
 * =====================================================================================
 */

#include "ConsoleAppender.h"

#include <cstdarg>
#include <cstdio>
#include <cstring>

#include <string>

#include "LogLevel.h"

#define LOG_BUFFER_SIZE 6000

void ConsoleAppender::log(
    std::string loggerName,
    LogLevel level,
    const char *fmt,
    va_list ap,
    const char *file,
    int line,
    const char *function)
{
    char buffer[LOG_BUFFER_SIZE];

    // time, levle and logger name
    snprintf(buffer, LOG_BUFFER_SIZE, "%s %s %s %s - ",
             getTimeNow().c_str(),
             getLogLevelString(level).c_str(),
             getPIDAndTID().c_str(),
             loggerName.c_str());

    // debug - location
    int len = strlen(buffer);

    if ((LOG_TRACE == level || LOG_DEBUG == level)
        && file != NULL && line != 0 && function != NULL) {
        snprintf(buffer + len, LOG_BUFFER_SIZE - len,
                 "file: %s, line: %d, function: %s, ",
                 file, line, function);
    }

    // log data
    len = strlen(buffer);
    vsnprintf(buffer + len, LOG_BUFFER_SIZE - len, fmt, ap);

    // 添加换行
    len = strlen(buffer);
    buffer[len++] = '\n';
    buffer[len] = '\0';

    len = strlen(buffer);

    // 去掉多余的空行
    while (buffer[len - 2] == '\n') {
        --len;
    }

    buffer[len] = '\0';

    // 输出到stderr
    fprintf(stderr, buffer);
}

