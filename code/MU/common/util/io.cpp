/*
 * =====================================================================================
 *
 *       Filename:  io.cpp
 *
 *    Description:  IO操作工具
 *
 *        Version:  1.0
 *        Created:  2011-04-29 16:04:03
 *  Last Modified:  2011-04-29 16:04:03
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lpc, lvpengcheng6300@gmail.com
 *        Company:  NDSL UESTC
 *
 * =====================================================================================
 */

#include "io.h"

#include <cerrno>
#include <cstddef>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

#include "log/log.h"

ssize_t util::io::readn(int fd, void *vptr, size_t n)
{
    if (NULL == vptr) {
        DEBUG_LOG("NULL pointer.");
        return -1;
    }

    size_t nleft;
    ssize_t nread;
    char *ptr = NULL;

    nleft = n;
    nread = 0;
    ptr = static_cast<char *>(vptr);

    while (nleft > 0) {
        if ((nread = read(fd, ptr, nleft)) < 0) {
            if (errno == EINTR) {
                nread = 0;
                continue;

            } else {
                DEBUG_LOG("Syscall Error: read. errno %d, %s.",
                          errno, strerror(errno));
                return -1;
            }

        } else if (nread == 0) {
            break;  // EOF
        }

        nleft -= nread;
        ptr += nread;
    }

    return (n - nleft);
}

ssize_t util::io::writen(int fd, const void *vptr, size_t n)
{
    if (NULL == vptr) {
        DEBUG_LOG("NULL pointer.");
        return -1;
    }

    size_t nleft;
    ssize_t nwritten;
    char *ptr = NULL;

    nleft = n;
    nwritten = 0;
    ptr = static_cast<char *>(const_cast<void *>(vptr));

    while (nleft > 0) {
        if ((nwritten = write(fd, ptr, nleft)) < 0) {
            if (errno == EINTR) {
                nwritten = 0;
                continue;

            } else {
                if (EAGAIN != errno) {
                    DEBUG_LOG("Syscall Error: write. errno %d, %s",
                              errno, strerror(errno));
                }

                return -1;
            }

        } else if (nwritten == 0) {
            break;
        }

        nleft -= nwritten;
        ptr += nwritten;
    }

    return (n - nleft);
}

int
util::io::setNonblock(int fd)
{
    int val;

    if ((val = fcntl(fd, F_GETFL, 0)) < 0) {
        DEBUG_LOG("Syscall Error: fcntl. %s.", strerror(errno));
        return val;
    }

    val |= O_NONBLOCK;

    if (fcntl(fd, F_SETFL, val) < 0) {
        DEBUG_LOG("Syscall Error: fcntl. %s.", strerror(errno));
        return -1;
    }

    return 0;
}

