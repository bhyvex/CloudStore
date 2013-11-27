/*
 * =========================================================================
 *
 *       Filename:  Reader.cpp
 *
 *    Description:  数据接收器
 *
 *        Version:  1.0
 *        Created:  2012-01-16 01:24:59
 *  Last Modified:  2012-01-16 01:24:59
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lpc, lvpengcheng6300@gmail.com
 *        Company:  NDSL UESTC
 *
 * =========================================================================
 */

#include "Reader.h"

#include "log/log.h"

int 
Reader::doRead(char *pBuf, size_t len)
{
    int rt = 0;

    rt = doReadInternal(pBuf, len);
    if (rt < 0) {
        ERROR_LOG("In Reader::doRead, Reader::doReadInternal error");
    } else if (0 == rt) {
        DEBUG_LOG("In Reader::doRead, EOF encountered");
    }

    return rt;
}

