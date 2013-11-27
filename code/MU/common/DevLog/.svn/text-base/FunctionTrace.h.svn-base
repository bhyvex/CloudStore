/*
 * =====================================================================================
 *
 *     FileName:  FunctionTrace.h
 *  Description:
 *      Version:  1.0
 *      Created:  Sun Jul  4 16:49:54 2010

 *     Revision:  none
 *     Compiler:  g++
 *
 *       Author:  ptz, ptz@uestc.edu.cn
 *      Company:  NDSL UESTC
 * =====================================================================================
 */

#ifndef _FUNCTIONTRACE_H_
#define _FUNCTIONTRACE_H_

#include <string>
//#include "DevLog.h"
class DevLog;

class FunctionTrace
{
public:
    FunctionTrace(DevLog *devLog, const std::string &func, const std::string &fileName);
    FunctionTrace(DevLog *devLog, const std::string &func, const std::string &fileName, const std::string &msg);
    ~FunctionTrace();

private:
    DevLog *m_pDevLog;
    std::string m_FuncName;
    std::string m_FileName;
    std::string m_Msg;
};

#endif
