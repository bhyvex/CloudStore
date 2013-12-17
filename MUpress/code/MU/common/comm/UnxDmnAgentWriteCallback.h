/*
 * =========================================================================
 *
 *       Filename:  UnxDmnAgentWriteCallback.h
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  2012-02-05 22:46:41
 *  Last Modified:  2012-02-05 22:46:41
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lpc, lvpengcheng6300@gmail.com
 *        Company:  NDSL UESTC
 *
 * =========================================================================
 */

#ifndef UNXDMNAGENTWRITECALLBACK_H_
#define UNXDMNAGENTWRITECALLBACK_H_

#include "WriteCallback.h"
#include "UnxDmnAgent.h"

#include "log/log.h"

class UnxDmnAgentWriteCallback : public WriteCallback
{
public:
    explicit UnxDmnAgentWriteCallback(UnxDmnAgent *pAgent) {
        m_pAgent = pAgent;
    }

    virtual ~UnxDmnAgentWriteCallback() {
        m_pAgent = NULL;
    }

    virtual int run(bool bResult, const DataContext *pContext) {
        if (NULL == m_pAgent) {
            ERROR_LOG("In UnxDmnAgentWriteCallback::run, m_pAgent == NULL");
            return -1;
        }

        return m_pAgent->writeCallback(bResult, pContext);
    }

private:
    UnxDmnAgent *m_pAgent;
};

#endif  // UNXDMNAGENTWRITECALLBACK_H_

