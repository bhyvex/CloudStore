/*
 * =====================================================================================
 * 
 *       Filename:  os.cpp
 * 
 *    Description:  操作系统常用命令
 * 
 *        Version:  1.0
 *        Created:  2011-07-22 02:59:56
 *  Last Modified:  2011-07-22 02:59:56
 *       Revision:  none
 *       Compiler:  gcc
 * 
 *         Author:  Shi Wei (sw), shiwei2012@gmail.com
 *        Company:  NDSL UESTC
 * 
 * =====================================================================================
 */

#include "os.h"

#include <string>

#include <cstring>
#include <cerrno>

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#include "log/log.h"

int
util::os::mkdir(const std::string &pathName,
        mode_t mode)
{
    int rt = 0;
    
    rt = ::mkdir(pathName.c_str(), mode);

    if (-1 == rt) {
        DEBUG_LOG("Syscall Error: mkdir. %s", strerror(errno));
        return -1;
    }

    return 0;
}

int
util::os::open(const std::string &path,
        int flags)
{
    int rt = 0;

    rt = ::open(path.c_str(), flags);

    if (-1 == rt) {
        DEBUG_LOG("Syscall Error: open. %s", strerror(errno));
        return -1;
    }

    return rt;
}

int
util::os::open(const std::string &path,
        int flags, mode_t mode)
{
    int rt = 0;

    rt = ::open(path.c_str(), flags, mode);

    if (-1 == rt) {
        DEBUG_LOG("Syscall Error: open. %s", strerror(errno));
        return -1;
    }

    return rt;
}

int
util::os::close(int fd)
{
    int rt = 0;

    rt = ::close(fd);
    if (-1 == rt) {
        DEBUG_LOG("Syscall Error: close. %s", strerror(errno));
        return -1;
    }

    return 0;
}

int
util::os::remove(const std::string &path)
{
    int rt = 0;

    rt = ::unlink(path.c_str());

    if (-1 == rt) {
        DEBUG_LOG("Syscall Error: unlink. %s", strerror(errno));
        return -1;
    }

    return 0;
}

int
util::os::ftruncate(int fd, off_t length)
{
    int rt = 0;

    rt = ::ftruncate(fd, length);
    if (-1 == rt) {
        DEBUG_LOG("Syscall Error: ftruncate. %s", strerror(errno));
        return -1;
    }

    return 0;
}


