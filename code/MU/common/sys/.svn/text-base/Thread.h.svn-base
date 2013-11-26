/*
 * =====================================================================================
 *
 *       Filename:  Thread.h
 *
 *    Description:  线程封装
 *
 *        Version:  1.0
 *        Created:  04/26/2011 04:51:08 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Shi Wei (sw), shiwei2012@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef COMMON_SYS_THREAD_H_
#define COMMON_SYS_THREAD_H_

#include <pthread.h>

#include "Mutex.h"
#include "Time.h"

#include "util/util.h"

/**
 * @brief 线程ID封装
 */
class ThreadID {
    public:
        ThreadID() {}
        explicit ThreadID(const pthread_t &tid);
        ~ThreadID() {}

        ThreadID(const ThreadID &tid);
        ThreadID& operator=(const ThreadID &tid);

        bool operator==(const ThreadID &tid);
        bool operator!=(const ThreadID &tid);

        /**
         * @brief 获取原始的类型为pthread_t的线程ID
         *
         * @return pthread_t类型的线程ID
         */
        pthread_t getRawID() const;

    private:
        pthread_t m_TID;
};

/**
 * @brief linux线程简单封装（抽象类）
 */
class Thread {
    public:
        Thread();
        virtual ~Thread();

        /**
         * @brief 启动线程
         *
         * @param stackSize 栈大小
         *
         * @return 
         */
        int start(int stackSize = 0);

        /**
         * @brief 线程是否存活，即底层线程是否运行
         *
         * @return 
         */
        bool isAlive() const;

        /**
         * @brief 等待线程结束
         *
         * @param valuePtr 用于存放线程返回状态的缓冲区地址
         *
         * @return 
         */
        int join(void **valuePtr = NULL);

        /**
         * @brief 分离线程
         *
         * @return 
         */
        int detach();

        /**
         * @brief 返回线程ID
         *
         * @return 
         */
        ThreadID id() const;

    protected:
        // can't copy
        Thread(const Thread &thread);
        Thread& operator=(const Thread &thread);

        /**
         * @brief 使调用线程自动放弃CPU，让其它线程运行
         */
        static void yield();

        /**
         * @brief 使调用线程睡眠
         *
         * @param timeout 睡眠时间
         */
        static void sleep(const Time &timeout);

        /**
         * @brief 派生类实现此方法以执行具体业务
         */
        virtual void run() = 0;

    private:
        /**
         * @brief 将运行标志置为false
         */
        void done();

        /**
         * @brief 传递给线程执行的函数
         *
         * @param arg
         *
         * @return 
         */
        static void* startHook(void *arg);
        
    protected:
        bool m_bRunning;  // 线程运行标志
        bool m_bDetached;  // 分离状态
        ThreadID m_ThreadID;  // 线程ID
};

#endif  // COMMON_SYS_THREAD_H_

