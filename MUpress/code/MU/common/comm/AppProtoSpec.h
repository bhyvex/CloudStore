/*
 * =========================================================================
 *
 *       Filename:  AppProtoSpec.h
 *
 *    Description:  应用层协议定义
 *
 *        Version:  1.0
 *        Created:  2012-01-16 17:50:59
 *  Last Modified:  2012-01-16 17:50:59
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lpc, lvpengcheng6300@gmail.com
 *        Company:  NDSL UESTC
 *
 * =========================================================================
 */

#ifndef APPPROTOSPEC_H_
#define APPPROTOSPEC_H_


/**
 * @brief 应用层协议定义
 */
class AppProtoSpec
{
public:
    AppProtoSpec() {

    }

    virtual ~AppProtoSpec() {

    }

    /**
     * @brief 协议头部长度
     *
     * @return 
     */
    virtual int getHeaderSize() = 0;

    /**
     * @brief 解析协议头，得到负载数据长度
     *
     * @param pHeader
     * @param headerLen
     *
     * @return 负载数据长度
     */
    virtual int getContentLength(const char *pHeader, int headerLen) = 0;

protected:
    // can't copy
    AppProtoSpec(const AppProtoSpec &proto);
    AppProtoSpec& operator=(const AppProtoSpec &proto);
};

#endif  // APPPROTOSPEC_H_

