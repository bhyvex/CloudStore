/*
 * =====================================================================================
 *
 *       Filename:  Mutex.h
 *
 *    Description:  pthread mutex封装
 *
 *        Version:  1.0
 *        Created:  04/25/2011 09:14:18 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Shi Wei (sw), shiwei2012@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef COMMON_SYS_MUTEX_H_
#define COMMON_SYS_MUTEX_H_

#include <pthread.h>

/**
 * @brief pthread mutex简单封装
 */
class Mutex {
    //friend class Cond;
public:

    Mutex();
    ~Mutex();

    /**
     * @brief 获取锁
     */
    void lock() const;

    /**
     * @brief trylock
     *
     * @return 是否成功获取锁
     */
    bool tryLock() const;

    /**
     * @brief 释放锁
     */
    void unlock() const;

    /**
     * @brief 获得底层phtread_mutex_t类型的互斥量，
     * 此方法只应由Cond调用
     *
     * @return 
     */
    pthread_mutex_t* getPthreadMutex() const;

private:
    // can't new
    Mutex(const Mutex &mutex);
    Mutex& operator=(const Mutex &mutex);

    // 让Cond在实现wait时模拟unlock和lock
    //struct LockState
    //{
    //    pthread_mutex_t* mutex;
    //};

    //void unlock(LockState *pLS) const;
    //void lock(LockState *pLS) const;

private:
    mutable pthread_mutex_t m_Mutex;
};

class LockGuard
{
public:
    virtual ~LockGuard();

    LockGuard(Mutex *pLock);

private:
    Mutex *_pLock;
};


#endif  // COMMON_SYS_MUTEX_H_

