/*
 * =========================================================================
 *
 *       Filename:  ReadCallback.h
 *
 *    Description:  Buffer数据读回调
 *
 *        Version:  1.0
 *        Created:  2012-01-16 18:01:01
 *  Last Modified:  2012-01-16 18:01:01
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Shi Wei (sw), shiwei2012@gmail.com
 *        Company:  NDSL UESTC
 *
 * =========================================================================
 */

#ifndef READCALLBACK_H_
#define READCALLBACK_H_

class ReadCallback
{
public:
    ReadCallback() {

    }

    virtual ~ReadCallback() {

    }

    virtual int run(const char *pHeader, int headerLen,
            char *pContent, int contentLen) = 0;
};

#endif  // READCALLBACK_H_

