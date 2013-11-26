#ifndef _BASEREQ_H_
#define _BASEREQ_H_
//#include <cstring>
//#include <iostream>
//#include "Agent.h"
#include "BaseHeader.h"
#include <netinet/in.h>
struct BaseRequest
{
    BaseRequest() {}
    virtual ~BaseRequest() {}
};

struct InReq
{
    MsgHeader m_msgHeader;
    char *ioBuf;
    sockaddr_in oppoAddr;
};
#endif
