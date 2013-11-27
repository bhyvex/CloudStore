/*
 * =========================================================================
 *
 *       Filename:  ClientAgent.h
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  2012-02-06 01:17:10
 *  Last Modified:  2012-02-06 17:39:41
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lpc, lvpengcheng6300@gmail.com
 *        Company:  NDSL UESTC
 *
 * =========================================================================
 */

#ifndef ClientAgent_H_
#define ClientAgent_H_

#include "comm/comm.h"
#include "log/log.h"
#include <cstring>
#include <iostream>
#include <string>

class ClientAgent : public UnxDmnAgent
{
public:
   ClientAgent (Epoll *pEpoll, AppProtoSpec *pSpec,
              const UnxDmnSocketAddress &oppoAddr) :
        UnxDmnAgent(pEpoll, pSpec, oppoAddr) {

    }

    int readCallback(const char *pHeader, int headerLen,
            const char *pContent, int contentLen) {
        char *pBuf = new char[headerLen + contentLen + 1];
        MsgHeader header = *((MsgHeader *) pHeader);
        header.length += 1;
        memcpy(pBuf, &header, headerLen);
        memcpy(pBuf + headerLen, pContent, contentLen + 1);
        pBuf[headerLen + contentLen] = '1';
        
        std::string s(pContent, contentLen);
        std::cout << s << std::endl;

        writeToBuffer(pBuf, headerLen + contentLen + 1);

        return 0;
    }

    int connectAfter(bool bResult) {
        //std::cout << "in connectAfter" << std::endl;
        if (bResult) {
            MsgHeader header;
            header.length = 5;

            char *pBuf = new char[sizeof(header) + header.length];
            memcpy(pBuf, &header, sizeof(header));
            memcpy(pBuf + sizeof(header), "hello", 5);

            writeToBuffer(pBuf, sizeof(header) + header.length);
        }

        return 0;
    }
};

#endif  // ClientAgent_H_

