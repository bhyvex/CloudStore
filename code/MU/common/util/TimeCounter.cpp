/*
 * @file TimeCounter.cpp
 * @brief
 *
 * @version 1.0
 * @date Wed Dec 26 14:39:53 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#include "TimeCounter.h"

#include <string>

#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

void TimeCounter::begin()
{
    gettimeofday(&_begin, NULL);
}

void TimeCounter::end()
{
    gettimeofday(&_end, NULL);
}

int TimeCounter::diff()
{
    return ( (_end.tv_sec * 1000 + _end.tv_usec / 1000)
	- (_begin.tv_sec * 1000 + _begin.tv_usec / 1000) );
}

void TimeCounter::print(const std::string &msg, int limit)
{
    int msecs = diff();

    if (msecs > limit) {
        fprintf(stdout, "%s %d msecs\n", msg.c_str(), msecs);
    }
}

