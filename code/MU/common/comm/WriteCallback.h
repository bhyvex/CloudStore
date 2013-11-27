/*
 * =========================================================================
 *
 *       Filename:  WriteCallback.h
 *
 *    Description:  Buffer数据写回调
 *
 *        Version:  1.0
 *        Created:  2012-01-16 17:58:46
 *  Last Modified:  2012-01-16 17:58:46
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lpc, lvpengcheng6300@gmail.com
 *        Company:  NDSL UESTC
 *
 * =========================================================================
 */

#ifndef WRITECALLBACK_H_
#define WRITECALLBACK_H_

struct DataContext;

/**
 * @brief Buffer数据写回调
 */
class WriteCallback
{
public:
    WriteCallback() {

    }

    virtual ~WriteCallback() {

    }

    virtual int run(bool bResult, const DataContext *pContext) = 0;
};

#endif  // WRITECALLBACK_H_

