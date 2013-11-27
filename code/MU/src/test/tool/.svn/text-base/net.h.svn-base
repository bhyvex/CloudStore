/*
 * @file net.h
 * @brief net utils
 *
 * @version 1.0
 * @date Wed Aug 15 00:16:39 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#ifndef MU_TEST_TOOL_NET_H_
#define MU_TEST_TOOL_NET_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <inttypes.h>

#include <string>

#include "protocol/protocol.h"
#include "comm/BaseHeader.h"
#include "comm/BaseReq.h"

namespace mu_test_tool_net
{

int readPacket(int connFd, InReq *pReq);

int sendPacket(int connFd, const MsgHeader &msg, const char *pBuf);

int connectTo(const std::string &ip, uint16_t port);

int connectTo(const struct in_addr &ip, uint16_t port);

int locate(
    const std::string &rsIp,
    uint16_t rsPort,
    uint64_t userId,
    struct in_addr *pAddr
);

}  // namespace mu_test_tool_net


#endif  // MU_TEST_TOOL_NET_H_

