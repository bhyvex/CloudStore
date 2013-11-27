/*
 * @file rm.cpp
 * @brief 
 * 
 * @version 1.0
 * @date Thu Jul 26 17:24:18 2012
 * 
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>

int rmdirRecursive(const std::string &path)
{
    int rt = 0;
    int error = 0;
    std::string entryName;
    std::string npath;

    DIR *pDir = ::opendir(path.c_str());

    // delete its children

    struct dirent *pEnt = NULL;

    while (NULL != (pEnt = ::readdir(pDir))) {
        entryName = pEnt->d_name;

        // omit "." and ".."
        // omit user info file in user root dir
        if (entryName == "."
            || entryName == "..") {
            continue;
        }

        npath = path + "/" + entryName;

        printf("npath %s\n", npath.c_str());

        if (DT_DIR == pEnt->d_type) {
            printf("dir\n");
            // directory, call myself to delete it

            rt = rmdirRecursive(npath);
            if (-1 == rt) {
                printf("rmdirRecursive failed\n");
                return -1;
            }

        } else {
            printf("not dir\n");
            // delete it directly
            rt = ::unlink(npath.c_str());

            if (-1 == rt) {
                printf("path %s, unlink() error, %s\n",
                          npath.c_str(), strerror(errno));
                return -1;
            }
        }
    }

    ::closedir(pDir);

    // delete path

    rt = ::rmdir(path.c_str());

    if (-1 == rt) {
        printf("path %s, rmdir() error, %s\n",
                  path.c_str(), strerror(errno));
        return 0;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    int rt = 0;

    rt = rmdirRecursive(argv[1]);

    if (-1 == rt) {
        printf("rm failed\n");
    }

    return 0;
}


