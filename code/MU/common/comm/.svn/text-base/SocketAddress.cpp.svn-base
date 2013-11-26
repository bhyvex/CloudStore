#include "SocketAddress.h"
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "BaseHeader.h"
#define IPV4LEN 15
#define IPV6LEN 39
#define MAXSOCKADDRLEN 100
#define ANYADDR "0"
int SocketAddress::ipType = IPV4;
SocketAddress::SocketAddress():
    port(0),m_Str("")
{
    if( ipType == IPV4 )
        ip = IPV4ANYADDR;

    else
        ip = IPV6ANYADDR;
}

SocketAddress::SocketAddress(const char *buf, unsigned short p)
{
    port = p;
    if( ipType == IPV4 )
        if( strlen( buf ) > IPV4LEN || strcmp( buf, ANYADDR) == 0)
            ip = IPV4ANYADDR;
        else
            ip = string( buf);
    else
    {
        if( strlen( buf ) > IPV6LEN || strcmp( buf, ANYADDR) == 0 )
            ip = IPV6ANYADDR;
        else
            ip = string( buf);
    }
}

void
SocketAddress::setPort(unsigned short p)
{
    port = p;
}

void
SocketAddress::setAddress( const char *buf, unsigned short p)
{
    port = p;
    if( ipType == IPV4 )
    {
        if( strlen( buf ) <= IPV4LEN && strcmp(buf, ANYADDR) != 0 )
            ip = string( buf);
    }
    else
    {
        if( strlen( buf ) <=  IPV6LEN && strcmp(buf, ANYADDR)!= 0 )
            ip = string( buf);
    }
}

string &
SocketAddress::convertToString( void )
{
    char buf[ MAXSOCKADDRLEN ] = { 0 };
    sprintf(buf, "%s %u", ip.c_str(), port);
    m_Str = buf;
    return m_Str;
}

bool
SocketAddress::ifAnyAddr(void)const
{
    if( ipType == IPV4 )
        if( IPV4ANYADDR == ip)
            return true;
        else
            return false;
    else if( IPV6ANYADDR == ip)
        return true;
    else
        return false;

}

bool
SocketAddress::operator == (const SocketAddress& h)const
{
    if( ip == h.ip && port == h.port )
        return true;
    else
        return false;
}

SocketAddress&
SocketAddress::operator=(const SocketAddress& h)
{
    ip = h.ip;
    port = h.port;
    m_Str = "";
    return *this;
}

sockaddr_in SocketAddress::getAddr()
{
    struct sockaddr_in addr;
    memset(&addr,0,sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    addr.sin_port = htons(port);
    return addr;
}

bool SocketAddress::getAddr(sockaddr_in &addr)
{
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    return inet_pton(AF_INET, ip.c_str(), &addr.sin_addr) == 1 ? true : false;
}
