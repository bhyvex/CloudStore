/*
 * =====================================================================================
 * 
 *       Filename:  os_path.cpp
 * 
 *    Description:  文件路径相关操作
 * 
 *        Version:  1.0
 *        Created:  2011-07-22 03:22:31
 *  Last Modified:  2011-07-22 03:22:31
 *       Revision:  none
 *       Compiler:  gcc
 * 
 *         Author:  lpc, lvpengcheng6300@gmail.com
 *        Company:  NDSL UESTC
 * 
 * =====================================================================================
 */

#include "os_path.h"

#include <string>

#include <cstring>
#include <cerrno>

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "log/log.h"

int
util::os::path::exists(
        const std::string &path,
        bool *pOutResult)
{
    // 参数检验
    if (NULL == pOutResult) {
        DEBUG_LOG("null pointer");
        return -1;
    }

    int rt = 0;

    struct stat st;

    memset(&st, 0, sizeof(st));

    rt = ::stat(path.c_str(), &st);

    if (0 == rt) {
        *pOutResult = true;
        return 0;
    }

    // -1 == rt
    if (ENOENT == errno || ENOTDIR == errno) {
        *pOutResult = false;
        return 0;
    }

    DEBUG_LOG("Syscall Error: stat. %s", strerror(errno));

    return -1;
}

int
util::os::path::isdir(
        const std::string &path,
        bool *pOutResult)
{
    // 参数检验
    if (NULL == pOutResult) {
        DEBUG_LOG("null ponter");
        return -1;
    }

    int rt = 0;

    struct stat st;

    memset(&st, 0, sizeof(st));

    rt = ::stat(path.c_str(), &st);

    if (0 == rt) {
        *pOutResult = (S_IFDIR == (st.st_mode & S_IFMT)) ? true : false;
        return 0;
    }

    // -1 == rt
    DEBUG_LOG("Syscall Error: stat. %s", strerror(errno));

    return -1;
}

int
util::os::path::isfile(
        const std::string &path,
        bool *pOutResult)
{
    if (NULL == pOutResult) {
        DEBUG_LOG("null pointer.");
        return -1;
    }

    int rt= 0;

    struct stat st;

    memset(&st, 0, sizeof(st));

    rt = ::stat(path.c_str(), &st);
    if (0 == rt) {
        *pOutResult = (S_IFREG == (st.st_mode & S_IFMT)) ? true : false;
        return 0;
    }

    DEBUG_LOG("Syscall Error: stat. %s", strerror(errno));

    return -1;
}

