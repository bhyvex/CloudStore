/*
 * =============================================================================
 * 
 *       Filename:  BaseHeader.h
 * 
 *    Description:  
 * 
 *        Version:  1.0
 *        Created:  2011-05-10 14:30:28
 *  Last Modified:  2011-05-10 14:30:28
 *       Revision:  none
 *       Compiler:  gcc
 * 
 *         Author:  lpc, lvpengcheng6300@gmail.com
 *        Company:  NDSL UESTC
 * 
 * =============================================================================
 */

#ifndef _BASE_HEADER_H_
#define _BASE_HEADER_H_

#include <sys/types.h>
#include <stdint.h>

const int CONNECTING = 0;
const int CONNECTED = 1;
const int CONNECTFAILED = -1;
const int MAXRECONNECTTIMES = 3;
#define IPV4 4
#define IPV6 6
#define IPV4ANYADDR "0.0.0"
#define IPV6ANYADDR "::1"

const unsigned int HEADER_SIZE = sizeof(uint32_t) * 5;

struct MsgHeader
{
    uint32_t cmd;	// 消息类型
    uint32_t length; //数据消息长度
    uint32_t error;
    uint32_t para1;
    uint32_t para2;

    //char length;

    MsgHeader() :
        cmd(0),
        length(0),
        error(0),
        para1(0),
        para2(0)
    {

    }

    ~MsgHeader() {

    }
};

#endif

