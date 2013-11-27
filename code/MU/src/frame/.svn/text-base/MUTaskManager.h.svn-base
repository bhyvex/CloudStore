/*
 * @file MUTaskManager.h
 * @brief Task manager of MU.
 *
 * @version 1.0
 * @date Wed Jun 27 21:28:33 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#ifndef MUTaskManager_H_
#define MUTaskManager_H_

#include "sys/Singleton.h"

#include <inttypes.h>

#include <map>
#include <list>
#include <set>

#include "log/log.h"
#include "MUTask.h"

class MUTaskManager : public Singleton<MUTaskManager>
{
    friend class Singleton<MUTaskManager>;

public:
    virtual ~MUTaskManager();

    /**
     * @brief Put a task into manager.
     *
     * @param taskId Id of task.
     * @param pTask Pointer to task.
     *
     * @return true if success, or false
     */
    bool put(uint64_t taskId, MUTask *pTask);

    /**
     * @brief Get a task by id.
     *
     * @param taskId Id to lookup.
     *
     * @return Pointer to task if found, or NULL
     */
    MUTask *get(uint64_t taskId);

    /**
     * @brief Remove a task from the manager.
     *
     * @param taskId TaskId to remove.
     */
    void remove(uint64_t taskId);

    /**
     * @brief factory method, create a new task
     *
     * @tparam ConcreteTask
     * @param pParentTask
     *
     * @return
     */
    template <typename ConcreteTask>
    ConcreteTask *createTask(MUTask *pParentTask);

    /**
     * @brief put a task into recycle list
     *
     * @param pTask
     */
    void recycle(MUTask *pTask);

    /**
     * @brief delete tasks in recycle list
     */
    void doRecycle();

protected:
    // can't new
    MUTaskManager();

private:
    std::map<uint64_t, MUTask *> m_TaskMap;
    //std::set<MUTask *> m_RecycleList;
    std::map<uint64_t, MUTask *> m_RecycleMap;
};

template <typename ConcreteTask>
ConcreteTask *
MUTaskManager::createTask(MUTask *pTask)
{
    ConcreteTask *pNTask = new ConcreteTask(pTask);

    if (!put(pNTask->getTaskId(), pNTask)) {
        WARN_LOG("can't put task %llu in task manager", pNTask->getTaskId());
    }

    return pNTask;
}


#endif  // MUTaskManager_H_

