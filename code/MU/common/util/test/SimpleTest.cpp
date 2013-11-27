/*
 * =====================================================================================
 *
 *       Filename:  SimpleTest.cpp
 *
 *    Description:  Basic test for util
 *
 *        Version:  1.0
 *        Created:  2011-04-28 11:48:55
 *  Last Modified:  2011-04-28 11:48:55
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lpc, lvpengcheng6300@gmail.com
 *        Company:  NDSL UESTC
 *
 * =====================================================================================
 */

#include "util/util.h"

#include <iostream>

int main() {
    std::string s = "2";

    std::cout << util::conv::conv<int, std::string>(s) << std::endl;

    int i = 4;
    std::cout << util::conv::conv<std::string, int>(i) << std::endl;

    return 0;
}

