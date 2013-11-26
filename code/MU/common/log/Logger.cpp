/*
 * =====================================================================================
 *
 *       Filename:  Logger.cpp
 *
 *    Description:  日志类实现
 *
 *        Version:  1.0
 *        Created:  04/24/2011 02:08:20 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Shi Wei (sw), shiwei2012@gmail.com
 *        Company:
 *
 * =====================================================================================
 */

#include "Logger.h"

#include <cstddef>
#include <cstdarg>

#include <list>
#include <string>

#include "Appender.h"
#include "LogLevel.h"

Logger::Logger()
    : m_Level(LOG_DEBUG)
{

}

Logger::~Logger()
{
    // 析构每个Appender
    for (std::list<Appender *>::iterator it = m_AppenderList.begin();
         it != m_AppenderList.end();
         ++it) {
        (*it)->destroy();
        delete(*it);
        *it = NULL;
    }

    m_AppenderList.clear();
}

void Logger::setLevel(LogLevel level)
{
    m_Level = level;
}

void Logger::setName(const std::string &name)
{
    m_Name = name;
}

void Logger::addAppender(Appender *apdr)
{
    m_AppenderList.push_back(apdr);
}

void Logger::doLog(
    LogLevel level,
    const char *fmt,
    va_list ap,
    const char *file,
    int line,
    const char *function)
{
    // 把日志写入到每个Appender
    for (std::list<Appender *>::iterator it = m_AppenderList.begin();
         it != m_AppenderList.end();
         ++it) {
        if (level == LOG_DEBUG) {  // 如果是调试信息
            if (NULL == file || NULL == function) {
                (*it)->log(m_Name, level, fmt, ap, "NULL", line, "NULL");

            } else {
                (*it)->log(m_Name, level, fmt, ap, file, line, function);
            }

        } else {
            (*it)->log(m_Name, level, fmt, ap);
        }
    }
}

void Logger::fatal(const char *fmt, ...)
{
    // 如果设定的日志级别比FATAL低，则记录日志
    if (LOG_FATAL >= m_Level) {
        va_list ap;

        va_start(ap, fmt);
        doLog(LOG_FATAL, fmt, ap);
        va_end(ap);
    }
}

void Logger::error(const char *fmt, ...)
{
    if (LOG_ERROR >= m_Level) {
        va_list ap;

        va_start(ap, fmt);
        doLog(LOG_ERROR, fmt, ap);
        va_end(ap);
    }
}

void Logger::warn(const char *fmt, ...)
{
    if (LOG_WARN >= m_Level) {
        va_list ap;

        va_start(ap, fmt);
        doLog(LOG_WARN, fmt, ap);
        va_end(ap);
    }
}

void Logger::info(const char *fmt, ...)
{
    if (LOG_INFO >= m_Level) {
        va_list ap;

        va_start(ap, fmt);
        doLog(LOG_INFO, fmt, ap);
        va_end(ap);
    }
}

void Logger::debug(
    const char *file,
    int line,
    const char *function,
    const char *fmt, ...)
{
    if (LOG_DEBUG >= m_Level) {
        va_list ap;

        va_start(ap, fmt);
        doLog(LOG_DEBUG, fmt, ap, file, line, function);
        va_end(ap);
    }
}

void Logger::trace(
    const char *file,
    int line,
    const char *function,
    const char *fmt, ...)
{
    if (LOG_TRACE >= m_Level) {
        va_list ap;

        va_start(ap, fmt);
        doLog(LOG_TRACE, fmt, ap, file, line, function);
        va_end(ap);
    }
}

