/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  test of DevLog class
 *
 *        Version:  1.0
 *        Created:  2010年05月18日 08时57分38秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), duanhancong@uestc.edu.cn
 *        Company:  NDSL UESTC
 *
 * =====================================================================================
 */
#include <pthread.h>
#include <string.h>//strerror
#include <iostream>
#include <sstream>
#include "Devlog/DevLog.h"
#include <unistd.h>
using namespace std;
void* func(void *arg);
DevLog *g_pDevLog = NULL;
int main()
{
	//initialize
	g_pDevLog = new DevLog("/home/ptz/dvcp/code/branch0.2.1/common/DevLog/test/logfile");
    g_pDevLog->init();
	g_pDevLog->setLogLevel(LEVEL_HIGH);

	int a=1;
		DEV_LOG_FUNC();
	DEV_LOG_ERROR("wa error found~~");
	if(a>0)
		DEV_LOG(LINFO, OUT_BOTH, "hello");
	else
	{

	}
	g_pDevLog->insertSeparatorLine();
	sleep(10);
	for(int i=0; i<10; ++i)
	{
		stringstream ss;
		ss << "thread " << i;
		string str = ss.str();
		pthread_t tid;
		int err = pthread_create(&tid, NULL, func, &str);
		if(err != 0)
		{
			DEV_LOG_ERROR(strerror(err));
		}
	}

    return 0;
}

void * func(void *arg)
{	
	DEV_LOG_FUNC_MSG(*(string*)arg);
	cout << *(string*)arg << endl;
	DEV_LOG(LINFO, OUT_BOTH, *(string*)arg);
	//g_pDevLog->writeLog(LINFO, OUT_BOTH, *(string*)arg);
	return NULL;
}
