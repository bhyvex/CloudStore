/*
 * @file RunControl.h
 * @brief Program start controller.
 *
 * @version 1.0
 * @date Thu Jun 28 21:39:36 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#ifndef _RUNCONCTROL_H_
#define _RUNCONCTROL_H_

#include "sys/sys.h"

class RunControl : public Singleton<RunControl>
{
    friend class Singleton<RunControl>;

public:
    void initialize();

    void run();

private:
    virtual ~RunControl();

    RunControl();

    void setResourceLimit();

    void initDevLog();

    void initLog();

    void initEpoll();

    void initThreadPool();

    void initOpLog();

    void initCSHeartbeat();

    void initCSListen();

    void initMigrationListen();

    void initRecycleTask();

    void initFilesystemRoot();

    void initCheckIdleUserTask();

    void startSystem();
};

#endif  // _RUNCONTROL_H_

