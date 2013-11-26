#ifndef SOCKETADDRESS_H_
#define SOCKETADDRESS_H_
#define IP_LENGTH 35
#define ADDRESS_LENGTH 21
#include <string>
#include <netinet/in.h>
#include "BaseHeader.h"
using namespace std;
//#define IPV4ANYADDR "0.0.0"
//#define IPV6ANYADDR "::1"
class SocketAddress
{
public:
    string ip;
    unsigned short port;
    string m_Str;
    static int ipType;

    SocketAddress();
    SocketAddress(const char *buf, unsigned short p);
    void setAddress( const char*, unsigned short);
    const char* getIP(void)const
    {
        return ip.c_str();
    };
    unsigned short getPort(void)const
    {
        return port;
    };
    sockaddr_in getAddr();
    bool getAddr(sockaddr_in &addr);	// added by niexw
    void setPort(unsigned short);
    string &convertToString( void );
    bool ifAnyAddr(void)const;
    bool operator == (const SocketAddress&)const;
    SocketAddress& operator = (const SocketAddress&);
};
#endif
