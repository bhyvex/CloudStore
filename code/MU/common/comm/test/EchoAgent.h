/*
 * =========================================================================
 *
 *       Filename:  EchoAgent.h
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  2012-02-06 01:17:10
 *  Last Modified:  2012-02-06 01:17:10
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lpc, lvpengcheng6300@gmail.com
 *        Company:  NDSL UESTC
 *
 * =========================================================================
 */

#ifndef EchoAgent_H_
#define EchoAgent_H_

#include "comm/comm.h"
#include "log/log.h"
#include <cstring>
#include <iostream>
#include <string>

class EchoAgent : public UnxDmnAgent
{
public:
    EchoAgent(Epoll *pEpoll, AppProtoSpec *pSpec,
              const UnxDmnSocket &socket,
              const UnxDmnSocketAddress &oppoAddr) :
        UnxDmnAgent(pEpoll, pSpec, socket, oppoAddr) {

    }

    int readCallback(const char *pHeader, int headerLen,
            const char *pContent, int contentLen) {
        char *pBuf = new char[headerLen + contentLen];
        memcpy(pBuf, pHeader, headerLen);
        memcpy(pBuf + headerLen, pContent, contentLen);

        //std::cout << "In readCallback" << endl;

        writeToBuffer(pBuf, headerLen + contentLen);

        return 0;
    }
};

#endif  // EchoAgent_H_

