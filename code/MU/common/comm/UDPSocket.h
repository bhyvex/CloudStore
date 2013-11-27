#ifndef UDPSOCKET_H
#define UDPSOCKET_H
#include <sys/uio.h>
#include <netinet/in.h>
#include "SocketAddress.h"
class UDPSocket
{
public:
    int sockFd;

    static int ipType;
    UDPSocket();

    int bindAddr(const SocketAddress& servaddr);
    int readSocket(char * buf, size_t len,struct sockaddr_in & cliaddr);
    int writeSocket(const char * buf, size_t len,const struct sockaddr_in& cliaddr);
    int generateSocket(void);
    int setNonblock(void);
    int enableReuseaddr(void);
    int enableBroadcast(void);
    int getSockFd(void)const;
    int closeSocket(void);
};
#endif
