/*
 * =====================================================================================
 *
 *       Filename:  SimpleTest.cpp
 *
 *    Description:  Basic TEST for log system
 *
 *        Version:  1.0
 *        Created:  04/24/2011 05:58:45 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lpc, lvpengcheng6300@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include "log/log.h"

class A {
    public:
        void f() {
            DEBUG_LOG("This is a log in a class.\n\n\n");
        }
};

int main() {
    Logger *rootLogger = LoggerFactory::getInstance()->getRootLogger();

    DEBUG_LOG("This is TEST %d for log system.", 1);

    rootLogger->fatal("Fata error log %d.\n", 0);
    rootLogger->error("Error log %d.\n", 1);
    rootLogger->warn("Warn log %d.\n", 2);
    rootLogger->info("Info log %d.\n", 3);
    rootLogger->debug(__FILE__, __LINE__, __FUNCTION__, "Debug log %d.\n", 4);

    A *pA = new A();
    pA->f();
    delete pA;

    delete LoggerFactory::getInstance();

    return 0;
}
