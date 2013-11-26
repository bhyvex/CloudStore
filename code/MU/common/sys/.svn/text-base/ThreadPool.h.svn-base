/*
 * =====================================================================================
 *
 *       Filename:  ThreadPool.h
 *
 *    Description:  线程池实现
 *
 *        Version:  1.0
 *        Created:  04/26/2011 11:11:25 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Shi Wei (sw), shiwei2012@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef COMMON_SYS_THREADPOOL_H_
#define COMMON_SYS_THREADPOOL_H_

#include <list>

#include "Thread.h"

// 最大空闲线程数
#define THREAD_MAX_IDLE_DEFAULT 15
// 最大线程总数
#define THREAD_MAX_TOTAL_DEFAULT 60

/**
 * @brief 线程池任务类接口
 */
class ThreadPoolWorkItem {
    public:
        ThreadPoolWorkItem() {}
        virtual ~ThreadPoolWorkItem() {}

        /**
         * @brief 在执行具体业务前的公共操作
         *
         * @return 
         */
        virtual int preProcess() {
            return 0;
        }

        /**
         * @brief 实现此方法以执行具体业务
         *
         * @return 
         */
        virtual int process() = 0;

        /**
         * @brief 执行具体业务后的公共操作
         *
         * @return 
         */
        virtual int postProcess() {
            return 0;
        }
};

/**
 * @brief 线程池实现
 */
class ThreadPool {
    public:
        /**
         * @brief 构造函数
         *
         * @param maxIdle 最大空闲线程数
         * @param maxTotal 最大线程总数
         */
        ThreadPool(int maxIdle = THREAD_MAX_IDLE_DEFAULT,
                int maxTotal = THREAD_MAX_TOTAL_DEFAULT);

        ~ThreadPool();

        /**
         * @brief 下发任务到线程池，即写入m_hWriteHandle
         *
         * @param pWorkItem 线程池任务指针
         *
         * @return 
         *      - 0 成功
         *      - ... 失败的errno
         */
        int postRequest(ThreadPoolWorkItem *pWorkItem);

        /**
         * @brief 返回线程池句柄，即m_hWriteHandle
         *
         * @return 线程池句柄
         */
        int getHandle() const;

        /**
         * @brief 启动线程池
         *
         * @return 
         */
        int start();

        /**
         * @brief 停止线程池
         *
         * @return 
         */
        int stop();

    protected:
        //can't copy
        ThreadPool(const ThreadPool &tp);
        ThreadPool& operator=(const ThreadPool &tp);

        /**
         * @brief 线程池中执行事件处理的线程
         */
        class EventHandlerThread : public Thread {
            public:
                EventHandlerThread(ThreadPool *pool);

            protected:
                virtual void run();
                
            private:
                ThreadPool *m_pThreadPool;
        };

        /**
         * @brief 线程池收到此任务则退出
         */
        class ThreadExitWorkItem : public ThreadPoolWorkItem {
            public:
                int process();
        };

    private:
        int m_hReadHandle;  // 管道读端
        int m_hWriteHandle;  // 管道写端
        int m_nMaxIdle;
        int m_nMaxTotal;
        std::list<Thread *> m_ThreadList;
        Mutex m_Mutex;  // to protect m_ThreadList;
};

#endif  // COMMON_SYS_THREADPOOL_H_

