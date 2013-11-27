/*
 * =========================================================================
 *
 *       Filename:  AppProto.h
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  2012-02-06 01:08:02
 *  Last Modified:  2012-02-06 01:08:02
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lpc, lvpengcheng6300@gmail.com
 *        Company:  NDSL UESTC
 *
 * =========================================================================
 */

#ifndef AppProto_H_
#define AppProto_H_

#include "log/log.h"
#include "comm/comm.h"

#include <cstring>

class AppProto : public AppProtoSpec
{
public:
    int getHeaderSize() {
        return sizeof(MsgHeader);
    }

    int getContentLength(const char *pHeader, int headerLen) {
        MsgHeader header;
        memcpy(&header, pHeader, headerLen);

        return header.length;
    }
};

#endif  // AppProto_H_

