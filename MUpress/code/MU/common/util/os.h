/*
 * =====================================================================================
 * 
 *       Filename:  os.h
 * 
 *    Description:  操作系统常用命令
 * 
 *        Version:  1.0
 *        Created:  2011-07-22 02:48:29
 *  Last Modified:  2011-07-22 02:48:29
 *       Revision:  none
 *       Compiler:  gcc
 * 
 *         Author:  lpc, lvpengcheng6300@gmail.com
 *        Company:  NDSL UESTC
 * 
 * =====================================================================================
 */

#ifndef COMMON_UTIL_OS_H_
#define COMMON_UTIL_OS_H_

#include <string>

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

namespace util {
    namespace os {
        /**
         * @brief 创建新目录的默认权限
         */
        const mode_t _DEFAULT_MODE_FOR_NEW_DIR = 
            S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH;

        /**
         * @brief 创建一个目录
         *
         * @param pathName 
         * @param mode 新建的目录的权限 - 默认权限755
         *
         * @return 
         */
        int mkdir(const std::string &pathName, 
                mode_t mode = _DEFAULT_MODE_FOR_NEW_DIR);

        int open(const std::string &path, int flags);

        int open(const std::string &path, int flags, mode_t mode);

        int close(int fd);

        /**
         * @brief 删除普通文件
         *
         * @param path
         *
         * @return 
         */
        int remove(const std::string &path);

        /**
         * @brief 截断文件描述指定的文件到length长度，
         *      如果length比当前文件长度大，则文件被扩展，填充0
         *
         * @param fd
         * @param length
         *
         * @return 
         */
        int ftruncate(int fd, off_t length = 0);

    }  // namespace os

} // namespace util


#endif  // COMMON_UTIL_OS_H_

