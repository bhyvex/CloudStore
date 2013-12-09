/*
 * =====================================================================================
 *
 *       Filename:  Cond.cpp
 *
 *    Description:  条件变量封装
 *
 *        Version:  1.0
 *        Created:  04/26/2011 10:20:29 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lpc, lvpengcheng6300@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include "Cond.h"

#include <pthread.h>

#include "log/log.h"

Cond::Cond(const Mutex &mutex) :
    m_Mutex(mutex) {
    pthread_condattr_t attr;
    int rt = pthread_condattr_init(&attr);

    if( 0 != rt ) {
        ERROR_LOG("Syscall Error: pthread_condattr_init.");
    }
    
    rt =  pthread_cond_init(&m_Cond, &attr);
    if( 0 != rt ) {
        ERROR_LOG("Syscall Error: pthread_cond_init.");
    }

    rt = pthread_condattr_destroy(&attr);
    if( 0 != rt ) {
        ERROR_LOG("Syscall Error: pthread_condattr_destroy.");
    }
}

Cond::~Cond() {
    pthread_cond_destroy(&m_Cond);
}

void Cond::notify() const {
    const int rt = pthread_cond_signal(&m_Cond);
    if ( 0 != rt ) {
        ERROR_LOG("Syscall Error: pthread_cond_signal.");
    }
}

void Cond::notifyAll() const {
    const int rt = pthread_cond_broadcast(&m_Cond);
    if( 0 != rt ) {
        ERROR_LOG("Syscall Error: pthread_cond_broadcast.");
    }
}

bool Cond::wait() const {
    int rt = pthread_cond_wait(&m_Cond, m_Mutex.getPthreadMutex());

    if (0 != rt) {
        ERROR_LOG("Syscall Error: pthread_cond_wait.");
        return false;
    }

    return true;
}

bool Cond::timedWait(const Time &timeout) const {
    if(timeout < Time::microSeconds(0)) {
        ERROR_LOG("Cond::timedWait: Invalid timeout argument.");
        return false;
    }

    struct timeval tv = Time::now(Time::Realtime) + timeout;
    struct timespec ts;
    ts.tv_sec = tv.tv_sec;
    ts.tv_nsec = tv.tv_usec * 1000;

    const int rc = pthread_cond_timedwait(
            &m_Cond, 
            m_Mutex.getPthreadMutex(), 
            &ts);

    if(rc != 0) {
        if(rc != ETIMEDOUT) {
            ERROR_LOG("Syscall Error: pthread_cond_timedwait.");
            return false;
        }
    }
    return true;
}

