/*
 * =====================================================================================
 *
 *       Filename:  SimpleTestThreadPool.cpp
 *
 *    Description:  Basic test for ThreadPool
 *
 *        Version:  1.0
 *        Created:  04/27/2011 10:32:47 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lpc, lvpengcheng6300@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include "sys/ThreadPool.h"

#include <iostream>

#include <unistd.h>

class EchoTask : public ThreadPoolWorkItem {
    public:
        int process() {
            std::cout << "Echo" << std::endl;
        }
};

int main() {
    ThreadPool *pool = new ThreadPool();
    pool->start();
    
    for (int i = 0; i < 10; ++i) {
        pool->postRequest(new EchoTask());
    }

    sleep(5);

    return 0;
}

