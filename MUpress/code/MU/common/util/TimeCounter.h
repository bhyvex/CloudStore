/*
 * @file TimeCounter.h
 * @brief A simple counter for elapsed time.
 *
 * @version 1.0
 * @date Wed Dec 26 14:35:29 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#ifndef TimeCounter_H_
#define TimeCounter_H_

#include <string>

#include <sys/time.h>

class TimeCounter
{
public:
    void begin();

    void end();

    int diff();

    void print(const std::string &msg, int limit);

private:
    struct timeval _begin;
    struct timeval _end;
};

#endif  // TimeCounter_H_

