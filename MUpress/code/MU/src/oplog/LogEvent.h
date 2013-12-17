/*
 * @file LogEvent.h
 * @brief Log record.
 *
 * @version 1.0
 * @date Mon Jul  9 15:16:40 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#ifndef _LogEvent_H_
#define _LogEvent_H_

#include "data/FileMeta.h"

#include <inttypes.h>

class LogEvent
{
public:
    virtual ~LogEvent();

    LogEvent();

    /**
     * @brief serialize this log record to pData
     *
     * @param pData
     *
     * @return true if a success, or false
     */
    bool serializeToString(std::string *pData);

public:
    uint64_t m_SeqNr;
    uint64_t m_UserId;
    uint64_t m_BucketId;
    std::string m_Token;
    uint32_t m_OpCode;
    std::string m_Path;
    std::string m_SrcPath;
    std::string m_DestPath;
    FileMeta m_FileMeta;

    // log record serialized using protobuf
    std::string m_SerializedLog;
};

#endif  // _LogEvent_H_

