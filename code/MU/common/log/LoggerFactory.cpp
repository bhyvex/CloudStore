/*
 * =====================================================================================
 *
 *       Filename:  LoggerFactory.cpp
 *
 *    Description:  日志工厂类
 *
 *        Version:  1.0
 *        Created:  04/24/2011 05:34:53 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lpc, lvpengcheng6300@gmail.com
 *        Company:
 *
 * =====================================================================================
 */

#include "LoggerFactory.h"

#include <cstdio>
#include <cstdlib>

#include <string>
#include <utility>

#include "LogLevel.h"
#include "ConsoleAppender.h"

// logger map
std::map<std::string, Logger *> LoggerFactory::m_LoggerMap;

LoggerFactory::LoggerFactory()
{

}

LoggerFactory::~LoggerFactory()
{
    // 析构所有Logger
    for (std::map<std::string, Logger *>::iterator it = m_LoggerMap.begin();
         it != m_LoggerMap.end();
         ++it) {
        delete(*it).second;
    }
}

int
LoggerFactory::initialize(LogLevel level)
{
    m_Level = level;

    // init ROOT Logger
    Logger *rootLogger = new Logger();
    rootLogger->setName("ROOT");
    rootLogger->setLevel(m_Level);

    Appender *p_Appender = new ConsoleAppender();

    if (p_Appender->init() != 0) {
        fprintf(stderr, "Error occurred while initializing the ROOT logger.\n");
        return -1;
    }

    rootLogger->addAppender(p_Appender);

    m_LoggerMap.insert(std::pair<std::string, Logger *>("ROOT", rootLogger));

    return 0;
}

Logger * LoggerFactory::getRootLogger()
{
    return m_LoggerMap["ROOT"];
}

