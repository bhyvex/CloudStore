/*
 * @file MakeDirs.cpp
 * @brief make directories in specified path
 *
 * @version 1.0
 * @date Mon Nov 26 12:16:54 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#include <string>
#include <sstream>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>

#define PATH_SEPARATOR_STR      "/"

void usage(const char *bin_name);

void *ThreadFunc(void *arg);

struct ThreadArg {
    int threadId;
    const char *path;
    int nfiles;
};

template <typename D, typename S>
D
conv(const S &s)
{
    std::stringstream ss;

    D d;

    ss << s;
    ss >> d;

    return d;
}

void
Usage(const char *binName)
{
    fprintf(stderr, "usage: %s path nfiles nthreads\n", binName);
}

void *
ThreadFunc(void *arg)
{
    struct ThreadArg *targ = (struct ThreadArg *) arg;

    int rc = 0;
    std::string basePath = std::string(targ->path) + PATH_SEPARATOR_STR
                           + conv<std::string, int>(targ->threadId);

    // mkdir
    rc = mkdir(basePath.c_str(), S_IRWXU);

    if (rc < 0) {
        fprintf(stderr, "thread %d, mkdir error, path %s, %s\n",
                targ->threadId, basePath.c_str(), strerror(errno));
        return (void *) - 1;
    }

    int i = 0;
    int fd = 0;
    std::string filePath;

    struct timeval begin;
    struct timeval end;

    gettimeofday(&begin, NULL);

    for (i = 0; i < targ->nfiles; ++i) {
        filePath = basePath + PATH_SEPARATOR_STR + conv<std::string, int>(i);

        fd = mkdir(filePath.c_str(), S_IRWXU);

        if (fd < 0) {
            fprintf(stderr, "thread %d, mkdir file error, path %s, %s\n",
                    targ->threadId, filePath.c_str(), strerror(errno));
            return (void *) - 1;
        }
    }

    gettimeofday(&end, NULL);

    int time = (end.tv_sec * 1000 + end.tv_usec / 1000) -
               (begin.tv_sec * 1000 + begin.tv_usec / 1000);

    printf("time of thread %d: %d\n", targ->threadId, time);

    return (void *) 0;
}

int
main(int argc, char *argv[])
{
    if (4 != argc) {
        Usage(argv[0]);
        return -1;
    }

    const char *path = argv[1];
    int nfiles = atoi(argv[2]);
    int nthreads = atoi(argv[3]);

    int nfilesPerThread = nfiles / nthreads;

    pthread_t *threads = new pthread_t[nthreads];
    ThreadArg *threadArgs = new ThreadArg[nthreads];

    int rc = 0;
    int i = 0;

    for (i = 0; i < nthreads; ++i) {
        ThreadArg *targ = threadArgs + i;
        targ->threadId = i;
        targ->path = path;
        targ->nfiles = nfilesPerThread;

        rc = pthread_create(threads + i, NULL, ThreadFunc, threadArgs + i);

        if (rc < 0) {
            fprintf(stderr, "create thread %d error, %s\n", i, strerror(errno));
            return -1;
        }
    }

    void *retval = NULL;

    for (i = 0; i < nthreads; ++i) {
        rc = pthread_join(*(threads + i), &retval);

        if (rc < 0) {
            fprintf(stderr, "join thread %d error, %s\n", i, strerror(errno));
            return -1;
        }
    }

    delete [] threads;
    delete [] threadArgs;

    return 0;
}


