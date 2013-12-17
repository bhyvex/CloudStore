/*
 * @file test_set.cpp
 * @brief 
 * 
 * @version 1.0
 * @date Thu Jul 26 12:02:37 2012
 * 
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#include <set>

int main(int argc, char *argv[])
{
    std::set<int> list;

    for (int i = 0; i < 5; ++i) {
        list.insert(i);
    }

    for (std::set<int>::iterator it = list.begin();
            it != list.end(); ) {
        *it = 0;
        list.erase(it++);
    }

    return 0;
}


