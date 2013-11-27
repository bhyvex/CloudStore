/*
 * @file opendir.c
 * @brief 
 * 
 * @version 1.0 
 * @date Wed Jul  4 14:50:42 2012
 * 
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#include <sys/types.h>
#include <dirent.h>

#include <stdio.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[])
{
    DIR *pDir = opendir(argv[1]);

    if (NULL == pDir) {
        if (ENOTDIR == errno) {
            printf("ENOTDIR\n");
        }
        if (ENOENT == errno) {
            printf("ENOENT\n");
        }
    }

    return 0;
}
