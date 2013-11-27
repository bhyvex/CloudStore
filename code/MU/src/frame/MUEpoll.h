/*
 * @file MUEpoll.h
 * @brief
 *
 * @version 1.0
 * @date Mon Dec  3 15:52:45 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#ifndef frame_MUEpoll_H_
#define frame_MUEpoll_H_

#include "comm/comm.h"

class MUEpoll : public Epoll
{
public:
    int doTask();
};


#endif  // frame_MUEpoll_H_
