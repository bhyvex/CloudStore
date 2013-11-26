/*
 * =====================================================================================
 *
 *       Filename:  io.h
 *
 *    Description:  IO操作工具
 *
 *        Version:  1.0
 *        Created:  2011-04-28 11:47:13
 *  Last Modified:  2011-04-28 11:47:13
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Shi Wei (sw), shiwei2012@gmail.com
 *        Company:  NDSL UESTC
 *
 * =====================================================================================
 */


#ifndef COMMON_UTIL_IO_H_
#define COMMON_UTIL_IO_H_

#include <sys/types.h>

namespace util {
    namespace io {
        /**
         * @brief 读n字节
         *
         * @param fd 描述符
         * @param vptr 存放读取数据的缓冲区
         * @param n 欲读取字节数
         *
         * @return 实际读取字节数
         */
        ssize_t readn(int fd, void *vptr, size_t n);

        /**
         * @brief 写n字节
         *
         * @param fd 描述符
         * @param vptr 欲写出的数据
         * @param n 欲写的字节数
         *
         * @return 实际写出的字节数
         */
        ssize_t writen(int fd, const void *vptr, size_t n);

        /**
         * @brief 将指定描述符设置非阻塞
         *
         * @param fd 要设置的描述符
         *
         * @return 
         *      -1 - 失败
         *      0  - 成功
         */
        int setNonblock(int fd);

    }  // namespace io

} // namespace util

#endif  // COMMON_UTIL_IO_H_

