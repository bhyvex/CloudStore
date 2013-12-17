/*
 * =====================================================================================
 *
 *       Filename:  conv.h
 *
 *    Description:  类型转换工具
 *
 *        Version:  1.0
 *        Created:  2011-04-28 11:41:12
 *  Last Modified:  2011-04-28 11:41:12
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lpc, lvpengcheng6300@gmail.com
 *        Company:  NDSL UESTC
 *
 * =====================================================================================
 */

#ifndef COMMON_UTIL_CONV_H_
#define COMMON_UTIL_CONV_H_

#include <sstream>

namespace util {
    namespace conv {
        /**
         * @brief 类型转换模板
         *
         * @tparam D 目标类型
         * @tparam S 源类型
         * @param s 源类型值
         *
         * @return 目标类型值
         */
        template <typename D, typename S>
        D conv(const S &s);

    }
}  // namespace util

template <typename D, typename S>
D util::conv::conv(const S &s) {
    std::stringstream ss;
    ss << s;
    D d;
    ss >> d;
    return d;
}

#endif  // COMMON_UTIL_CONV_H_

