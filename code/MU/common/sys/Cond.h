/*
 * =====================================================================================
 *
 *       Filename:  Cond.h
 *
 *    Description:  条件变量封装
 *
 *        Version:  1.0
 *        Created:  04/26/2011 09:23:24 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lpc, lvpengcheng6300@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef COMMON_SYS_COND_H_
#define COMMON_SYS_COND_H_

#include <pthread.h>
#include <cerrno>

#include "Mutex.h"
#include "Time.h"

#include "log/log.h"

/** 
 * @brief linux线程条件变量
 */
class Cond {
public:

    //Cond();
    Cond(const Mutex &mutex);
    ~Cond();

    /** 
     * @brief 唤醒等待线程
     */
    void notify() const;

    /** 
     * @brief 唤醒所有等待线程
     */
    void notifyAll() const;

    /**
     * @brief 线程阻塞等待条件
     *
     * @return 
     */
    bool wait() const;        

    /**
     * @brief 线程阻塞等待，条件发生或超时返回
     *
     * @param timeout 超时时间
     *
     * @return 
     */
    bool timedWait(const Time &timeout) const;

    /** 
     * @brief 线程阻塞等待 
     * 
     * @param mutex 已锁住的互斥量
     * 
     * @return 
     */
    //inline bool 
    //wait(const Mutex& mutex) {
    //    return waitImpl(mutex);
    //}

    /** 
     * @brief 线程阻塞等待,到达定时时间自动运行
     * 
     * @param mutex 已锁住的互斥量
     * @param timeout 超时时间
     * 
     * @return 
     */
    //inline bool
    //timedWait(const Mutex &mutex, const Time& timeout) {
    //    return timedWaitImpl(mutex, timeout);
    //}

private:

    //bool waitImpl(const Mutex &mutex);
    //bool timedWaitImpl(const Mutex &mutex, const Time& timeout);

    mutable pthread_cond_t m_Cond;
    const Mutex &m_Mutex;
};

/*
inline bool 
Cond::waitImpl(const Mutex& mutex) {
    typedef Mutex::LockState LockState;
    
    LockState state;
    mutex.unlock(&state);
    const int rc = pthread_cond_wait(&m_Cond, state.mutex);
    mutex.lock(&state);
    
    if( 0 != rc ) {
        DEBUG_LOG("Syscall Error: pthread_mutex_wait.");
        return false;
    } 

    return true;
}

inline bool
Cond::timedWaitImpl(const Mutex& mutex, const Time& timeout) {
    if(timeout < Time::microSeconds(0)) {
        DEBUG_LOG("Cond::timedWait: Invalid timeout argument.");
        return false;
    }

    typedef Mutex::LockState LockState;
    
    LockState state;
    mutex.unlock(&state);
   
    timeval tv = Time::now(Time::Realtime) + timeout;
    timespec ts;
    ts.tv_sec = tv.tv_sec;
    ts.tv_nsec = tv.tv_usec * 1000;

    const int rc = pthread_cond_timedwait(&m_Cond, state.mutex, &ts);
    mutex.lock(&state);
    
    if(rc != 0) {
        if(rc != ETIMEDOUT) {
            DEBUG_LOG("Syscall Error: pthread_cond_timedwait.");
            return false;
        }
    }
    return true;
}
*/

#endif  // COMMON_SYS_COND_H_

