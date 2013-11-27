/*
 * =====================================================================================
 *
 *       Filename:  BaseTask.h
 *
 *    Description:  the virtual base class of all task class
 *
 *        Version:  1.0
 *        Created:  2010年03月26日 14时56分50秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  ptz, ptz_007@uestc.edu.cn
 *        Company:  NDSL UESTC
 *
 * =====================================================================================
 */
#ifndef _BASETASK_H_
#define _BASETASK_H_
#include "BaseReq.h"

class BaseTask
{
public:
    BaseTask() {}
    virtual ~BaseTask() {}
    virtual int recvReq(BaseRequest*);

    virtual void readBack(InReq& req) {}
    virtual void writeBack(bool result) {}
    //virtual int getTaskID();
//    virtual int execute(TaskPara *para){}
};

#endif
