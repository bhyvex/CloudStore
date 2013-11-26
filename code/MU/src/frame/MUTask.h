/*
 * @file MUTask.h
 * @brief Base Task definition for Mu module.
 *
 * @version 1.0
 * @date Tue Jun 26 16:52:55 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#ifndef MUTask_H_
#define MUTask_H_

#include "comm/comm.h"

#include "MUTCPAgent.h"
#include "MUMacros.h"
#include "protocol/MUMacros.h"

#include <string>

#include <inttypes.h>

class MUTCPAgent;
class MUTimer;
class MUWorkItem;

class MUTask : public BaseTask
{
public:
    virtual ~MUTask();

    MUTask(MUTask *pTask);

    /**
     * @brief when this task's subcomponents being destoried,
     *          they call this method to notify this task,
     *          then this task can decide whether itself should be deleted.
     *
     * @param pChannel subcomponent
     */
    virtual void destroy(MUTCPAgent *pChannel);

    virtual void destroy(MUTask *pChannel);

    virtual void destroy(MUTimer *pChannel);

    /**
     * @brief get the unique id of this task object
     *
     * @return id of this task
     */
    uint64_t getTaskId();

    /**
     * @brief called by communication agent.
     *
     * @param pAgent Channel where data comes from.
     * @param req data packet
     *
     * @return 0 if a success, or -1
     */
    virtual int next(MUTCPAgent *pAgent, const InReq &req);

    /**
     * @brief called by communication agent when connection established
     *          or connect failed.
     *
     * @param pAgent
     * @param bConn
     *
     * @return
     */
    virtual int connectCallback(MUTCPAgent *pAgent, bool bConn);

    /**
     * @brief called by thread pool dispatcher
     *
     * @param pItem thread pool request
     *
     * @return  0 if a success, or -1
     */
    virtual int next(MUWorkItem *pItem);

    /**
     * @brief called by timer
     *
     * @param pTimer Channel where the callback comes from.
     * @param times Expirations ocurred.
     *
     * @return 0 if a success, or -1
     */
    virtual int next(MUTimer *pTimer, uint64_t times);

    /**
     * @brief called by another task
     *
     * @param pTask
     *
     * @return
     */
    virtual int next(MUTask *pTask);

    virtual bool error();

    void setParent(MUTask *pTask);

    int getRefCnt();

protected:
    /**
     * @brief generate an unique id for this task
     */
    void generateTaskId();

    /**
     * @brief send simple data packet without extra data.
     *
     * @param pAgent
     * @param cmd
     * @param error
     */
    void sendSimplePacket(MUTCPAgent *pAgent,
                          uint32_t cmd, uint32_t error = MU_OK);

    /**
     * @brief add ref cnt
     */
    void ref();

    /**
     * @brief sub ref cnt
     */
    void release();

    /**
    * @brief 1) if system is normal, just send it to thread pool.
    *        2) if system is locked, place it to a container
    *          and execute it later.
    *
    * @param pItem
    */
    void sendItem(MUWorkItem *pItem);

    /**
     * @brief close socket connection and release associated agent
     *
     * @param pAgent
     */
    void closeAgent(MUTCPAgent *pAgent);

    /**
     * @brief put itself into recycle list
     */
    void recycle();

    /**
     * @brief put a sub-task into recycle list
     *
     * @param pTask
     */
    void recycle(MUTask *pTask);

    /**
     * @brief delete an agent
     *
     * @param pAgent
     */
    void recycle(MUTCPAgent *pAgent);

    /**
     * @brief delete a timer
     *
     * @param pTimer
     */
    void recycle(MUTimer *pTimer);

protected:
    MUTask *m_pParent;  // parent task
    int m_CurrentState;

private:
    uint64_t m_TaskId;  // unique id of this task
    int m_RefCnt;
};

#endif  // MUTask_H_

