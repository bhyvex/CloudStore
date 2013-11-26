/*
 * =====================================================================================
 *
 *       Filename:  Thread.cpp
 *
 *    Description:  linux线程简单封装
 *
 *        Version:  1.0
 *        Created:  04/26/2011 05:36:32 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Shi Wei (sw), shiwei2012@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include "Thread.h"

#include <climits>

#include <pthread.h>
#include <sched.h>
#include <time.h>

#include "Lock.h"
#include "log/log.h"

Thread::Thread() :
    m_bRunning(false),
    m_bDetached(false) {
    
}

Thread::~Thread() {

}

void* Thread::startHook(void *arg) {
    Thread *pThread = static_cast<Thread *>(arg);
    pThread->run();
    pThread->done();
}

int Thread::start(int stackSize) {
    if (m_bRunning) {
        DEBUG_LOG("Try to start an already started thread.");
        return -1;
    }

    pthread_t tid;

    if (stackSize > 0) {
        pthread_attr_t attr;
        int rt = pthread_attr_init(&attr);

        if (0 != rt) {
            DEBUG_LOG("Syscall Error: pthread_attr_init.");
            return -1;
        }

        if (stackSize < PTHREAD_STACK_MIN) {
            stackSize = PTHREAD_STACK_MIN;
        }

        rt = pthread_attr_setstacksize(&attr, stackSize);

        if (0 != rt) {
            DEBUG_LOG("Syscall Error: pthread_attr_setstatcksize.");
            return -1;
        }

        rt = pthread_create(&tid, &attr, startHook, this);

        if (0 != rt) {
            DEBUG_LOG("Syscall Error: pthread_create.");
            return -1;
        }
    } else {
        const int rt = pthread_create(&tid, NULL, startHook, this);

        if (0 != rt) {
            DEBUG_LOG("Syscall Error: pthread_create. %s", strerror(errno));
            return -1;
        }
    }

    m_ThreadID = ThreadID(tid);

    if (m_bDetached) {
        detach();
    }

    m_bRunning = true;

    return 0;

    // release lock
}

bool Thread::isAlive() const {
    return m_bRunning;
}

void Thread::done() {
    m_bRunning = false;
}

int Thread::join(void **valuePtr) {
    if (m_bDetached) {
        DEBUG_LOG("Try to join a detached thread.");
        return -1;
    }

    const int rt = pthread_join(m_ThreadID.getRawID(), valuePtr);

    if (0 != rt) {
        DEBUG_LOG("Syscall Error: pthread_join.");
        return -1;
    }

    return 0;
}

int Thread::detach() {
    if (m_bDetached) {
        DEBUG_LOG("Try to detach an already detached thread.");
        return -1;
    }

    const int rt = pthread_detach(m_ThreadID.getRawID());

    if (0 != rt) {
        DEBUG_LOG("Syscall Error: pthread_detach.");
        return -1;
    }

    return 0;
}

ThreadID Thread::id() const {
    return m_ThreadID;
}

void Thread::yield() {
    sched_yield();
}

void Thread::sleep(const Time &timeout) {
    struct timeval tv = timeout;
    struct timespec ts;
    ts.tv_sec = tv.tv_sec;
    ts.tv_nsec = tv.tv_usec * 1000;
    nanosleep(&ts, 0);
}

ThreadID::ThreadID(const pthread_t &tid) :
    m_TID(tid) {
}

ThreadID::ThreadID(const ThreadID &tid) {
    *this = tid;
}

ThreadID& ThreadID::operator=(const ThreadID &tid) {
    this->m_TID = tid.m_TID;
    return *this;
}

bool ThreadID::operator==(const ThreadID &tid) {
    const int rt = pthread_equal(m_TID, tid.m_TID);

    if (0 == rt) {  // not equal
        return false;
    }

    return true;
}

bool ThreadID::operator!=(const ThreadID &tid) {
    return !(*this == tid);
}

pthread_t ThreadID::getRawID() const {
    return m_TID;
}

