/*
 * @file LogFileDAO.h
 * @brief Log file operations.
 *
 * @version 1.0
 * @date Tue Jul 10 14:20:06 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#ifndef _oplog_LogFileDAO_H_
#define _oplog_LogFileDAO_H_

#include "frame/MUDAO.h"
#include "frame/ReturnStatus.h"

#include <list>
#include <map>
#include <string>

#include <inttypes.h>

class LogFileDAO : public MUDAO
{
public:
    virtual ~LogFileDAO();

    LogFileDAO();

    ReturnStatus checkLogFileSize(
        const std::list<uint64_t> &bucketList,
        std::map<uint64_t, uint64_t> *pLogMap);

private:
    std::string logPath(uint64_t bucketId);
};


#endif  // _oplog_LogFileDAO_H_

