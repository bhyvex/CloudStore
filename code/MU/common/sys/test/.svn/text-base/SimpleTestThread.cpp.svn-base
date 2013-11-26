/*
 * =====================================================================================
 *
 *       Filename:  SimpleTestThread.cpp
 *
 *    Description:  Basic test for Thread 
 *
 *        Version:  1.0
 *        Created:  04/26/2011 09:24:59 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Shi Wei (sw), shiwei2012@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */


#include "sys/Thread.h"

#include "log/log.h"

#include <cstdlib>
#include <unistd.h>

class ThreadA : public Thread {
    public:
        void run() {
            for (; ; ) {
                DEBUG_LOG("I am Thread A.");
                Thread::sleep(Time::seconds(1));
            }
        }
};

class ThreadB : public Thread {
    public:
        void run() {
            for (; ; ) {
                DEBUG_LOG("I am Thread B.");
                Thread::sleep(Time::seconds(1));
                //Thread::sleep(Time::microSeconds(random()));
            }
        }
};

int main() {
    Thread *pA = new ThreadA();
    Thread *pB = new ThreadB();

    pA->start();
    pB->start();

    sleep(10);

    return 0;
}

