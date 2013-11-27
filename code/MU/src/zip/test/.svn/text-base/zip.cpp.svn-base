/*
 * @file zip.cpp
 * @brief compress a directory
 * 
 * @version 1.0
 * @date Thu Aug 16 00:24:41 2012
 * 
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#include "zip/Zip.h"

#include <stdio.h>
#include <errno.h>
#include <string.h>

int
main(int argc, char *argv[])
{
    if (3 != argc) {
        fprintf(stderr, "usage: %s archive_name directory_name\n",
                argv[0]);
        return -1;
    }

    Zip *pZip = new Zip(argv[1]);

    int rt = 0;

    rt = pZip->compress(argv[2]);

    delete pZip;
    pZip = NULL;

    if (-1 == rt) {
        fprintf(stderr, "compress failed\n");
        return -1;
    }
        
    fprintf(stderr, "compress successfully\n");

    return 0;
}
