/*
 * =====================================================================================
 * 
 *       Filename:  os_path.h
 * 
 *    Description:  与文件路径相关操作
 * 
 *        Version:  1.0
 *        Created:  2011-07-22 03:07:04
 *  Last Modified:  2011-07-22 03:07:04
 *       Revision:  none
 *       Compiler:  gcc
 * 
 *         Author:  Shi Wei (sw), shiwei2012@gmail.com
 *        Company:  NDSL UESTC
 * 
 * =====================================================================================
 */

#ifndef COMMON_UTIL_OS_PATH_H_
#define COMMON_UTIL_OS_PATH_H_

#include <string>

#include <sys/stat.h>
#include <sys/types.h>

namespace util 
{
    namespace os 
    {
        namespace path 
        {
            /**
             * @brief 指定路径是否存在
             *
             * @param path
             * @param pOutResult 返回结果
             *
             * @return 
             *      -1 - 内部错误
             *       0 - 函数调用成功
             */
            int exists(const std::string &path, bool *pOutResult);

            /**
             * @brief 指定路径是否是一个目录
             *
             * @param path
             * @param pOutResult 返回结果
             *
             * @return 
             *      -1 - 
             *       0 - 
             */
            int isdir(const std::string &path, bool *pOutResult);

            /**
             * @brief 指定路径是否是普通文件
             *
             * @param path
             * @param pOutResult
             *
             * @return 
             */
            int isfile(const std::string &path, bool *pOutResult);
        }  // namespace path
    }  // namespace os

} // namespace util



#endif  // COMMON_UTIL_OS_PATH_H_

