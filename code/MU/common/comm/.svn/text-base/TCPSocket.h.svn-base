#ifndef TCPSOCKET_H
#define TCPSOCKET_H
#include <sys/uio.h>
#include "SocketAddress.h"
class TCPSocket
{
public:
    int sockFd;

    static int ipType;
    TCPSocket();
    TCPSocket(int fd);
    int readSocket(char* buf, size_t len);
    int readnSocket(char *vptr, unsigned int n);
    int writeSocket(const char* buf, size_t len);
    int generateSocket(void);
    int setNonblock(void);
    int enableReuseaddr(void);
    int disableLinger(void);
    int disableNagle(void);
    int getSockFd(void)const;
    int bindAddr(SocketAddress&);
    int listenOn(int qs);
    int acceptConnection(SocketAddress&);
    int writevSocket(const struct iovec*, size_t);
    int closeSocket(void);
    int connectSocket(SocketAddress&);
    int setKeepAlive( void );
    //for debug
    int Peek(char* buf, size_t size);
    //for debug end
    void ThrowData(unsigned int size);

};

#endif
