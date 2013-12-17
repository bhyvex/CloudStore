/*
 * =========================================================================
 *
 *       Filename:  UnxDmnAgentReadCallback.h
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  2012-02-05 22:36:45
 *  Last Modified:  2012-02-05 22:36:45
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lpc, lvpengcheng6300@gmail.com
 *        Company:  NDSL UESTC
 *
 * =========================================================================
 */

#ifndef UNXDMNAGENTREADCALLBACK_H_
#define UNXDMNAGENTREADCALLBACK_H_

#include "ReadCallback.h"

#include "log/log.h"

class UnxDmnAgent;

class UnxDmnAgentReadCallback : public ReadCallback
{
public:
    explicit UnxDmnAgentReadCallback(UnxDmnAgent *pAgent) {
        m_pAgent = pAgent;
    }

    virtual ~UnxDmnAgentReadCallback() {
        m_pAgent = NULL;
    }

    virtual int run(const char *pHeader, int headerLen,
                    char *pContent, int contentLen) {
        if (NULL == m_pAgent) {
            ERROR_LOG("In UnxDmnAgentWriteCallback::run, m_pAgent == NULL");
            return -1;
        }

        return m_pAgent->readCallback(
                pHeader, headerLen, pContent, contentLen);
    }
private:
    UnxDmnAgent *m_pAgent;
};

#endif  // UNXDMNAGENTREADCALLBACK_H_

