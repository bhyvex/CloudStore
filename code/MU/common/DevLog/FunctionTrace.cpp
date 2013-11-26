/*
 * =====================================================================================
 *
 *     FileName:  FunctionTrace.cpp
 *  Description:  implementation of FunctionTrace class
 *      Version:  1.0
 *      Created:  Sun Jul  4 16:49:54 2010

 *     Revision:  none
 *     Compiler:  g++
 *
 *       Author:  ptz, ptz@uestc.edu.cn
 *      Company:  NDSL UESTC
 * =====================================================================================
 */

#include "FunctionTrace.h"
#include "DevLog.h"
#include "comm/Error.h"

using std::string;

FunctionTrace::FunctionTrace(DevLog *devLog, const string &func, const string &fileName):
        m_pDevLog(devLog),
        m_FuncName(func),
        m_FileName(fileName)
{
    if (m_pDevLog != NULL)
    {
        string logStr("Function \"" + m_FuncName + "\" entered in " + m_FileName);
        m_pDevLog->writeLog(LFUNC, OUT_FILE, logStr);
    }

}

FunctionTrace::FunctionTrace(DevLog *devLog, const string &func, const string &fileName, const std::string &msg):
        m_pDevLog(devLog),
        m_FuncName(func),
        m_FileName(fileName),
        m_Msg(msg)
{
    if (m_pDevLog != NULL)
    {
        string logStr("Function \"" + m_FuncName + "\" entered in " + m_FileName + ": " + m_Msg);
        m_pDevLog->writeLog(LFUNC, OUT_FILE, logStr);
    }

}

FunctionTrace::~FunctionTrace()
{
    try
    {
        if (m_pDevLog != NULL)
        {
            string logStr("Function \"" + m_FuncName + "\" exited in " + m_FileName + ": " + m_Msg);
            m_pDevLog->writeLog(LFUNC, OUT_FILE, logStr);
        }
        m_pDevLog = NULL;
    }
    catch (...)
    {
        RED_MSG("~FunctionTrace exception");
    }
}

