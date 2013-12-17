/*
 * @file statvfs.c
 * @brief 
 * 
 * @version 1.0
 * @date Mon Jul 23 19:48:18 2012
 * 
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#include <sys/statvfs.h>

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

int main()
{
    struct statvfs st;

    int rt = 0;

    rt = statvfs(".", &st);
    if (-1 == rt) {
        printf("statvfs() error, %s\n", strerror(errno));
        exit(1);
    }

    printf("f_bsize %lu\n", st.f_bsize);
    printf("f_frsize %lu\n", st.f_frsize);
    printf("f_blocks %lu\n", st.f_blocks);
    printf("f_bfree %lu\n", st.f_bfree);
    printf("f_bavail %lu\n", st.f_bavail);
    printf("f_files %lu\n", st.f_files);
    printf("f_ffree %lu\n", st.f_ffree);
    printf("f_favail %lu\n", st.f_favail);

    uint64_t total_c = ((uint64_t) st.f_bsize) * st.f_blocks;
    uint64_t total_r = st.f_bsize * st.f_blocks;

    printf("total %lu\n", st.f_bsize * st.f_blocks);
    printf("total %llu\n", st.f_bsize * st.f_blocks);
    printf("total %lu\n", ((uint64_t) st.f_bsize) * st.f_blocks);
    printf("total %llu\n", ((uint64_t) st.f_bsize) * st.f_blocks);
    printf("total_c %lu\n", total_c);
    printf("total_c %llu\n", total_c);
    printf("total_r %lu\n", total_r);
    printf("total_r %llu\n", total_r);

    printf("total %lu MB\n", st.f_bsize * st.f_blocks / 1024 / 1024);
    printf("total %llu MB\n", ((uint64_t) st.f_bsize) * st.f_blocks / 1024 / 1024);
    printf("total %lu MB\n", st.f_bsize * st.f_blocks / 1024 / 1024);
    printf("total %llu MB\n", ((uint64_t) st.f_bsize) * st.f_blocks / 1024 / 1024);
    printf("total_c %lu MB\n", total_c / 1024 / 1024);
    printf("total_c %llu MB\n", total_c / 1024 / 1024);
    printf("total_r %lu MB\n", total_r / 1024 / 1024);
    printf("total_r %llu MB\n", total_r / 1024 / 1024);

    return 0;
}


