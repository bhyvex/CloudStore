/*
 * @file Zip.cpp
 * @brief create a zip archive or uncompress a zip archive.
 *
 * @version 1.0
 * @date Wed Aug 15 21:00:08 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#include "Zip.h"

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <inttypes.h>
#include <errno.h>
#include <string.h>

#include <string>

#include <zip.h>

#define ZIP_PATH_SEPARATOR_STR      "/"
#define ZIP_DENTRY_CURRENT_DIR      "."
#define ZIP_DENTRY_PARENT_DIR       ".."
#define ZIP_BUFFER_SIZE             4096

Zip::~Zip()
{
    int rt = 0;

    if (NULL != _pZip) {
        rt = closeArchive();

        if (-1 == rt) {
            fprintf(stderr, "close archive %s failed\n",
                    _archivePath.c_str());
        }
    }
}

Zip::Zip(const std::string &path)
{
    _archivePath = path;
    _pZip = NULL;
}

int
Zip::compress(const std::string &srcPath)
{
    int rt = 0;

    // must be an absolute path

    if (srcPath.substr(0, 1) != ZIP_PATH_SEPARATOR_STR) {
        fprintf(stderr, "%s is not an absolute path\n",
                srcPath.c_str());
        return -1;
    }

    // must be a directory

    struct stat st;

    rt = ::stat(srcPath.c_str(), &st);

    if (-1 == rt) {
        fprintf(stderr, "path %s, stat() error, %s\n",
                srcPath.c_str(), strerror(errno));
        return -1;
    }

    if (!S_ISDIR(st.st_mode)) {
        fprintf(stderr, "path %s is not a directory\n",
                srcPath.c_str());
        return -1;
    }

    // set root path

    if (srcPath == ZIP_PATH_SEPARATOR_STR) {
        // root "/"
        _rootPath = srcPath;

    } else if (srcPath.substr(srcPath.length() - 1) == ZIP_PATH_SEPARATOR_STR) {
        _rootPath = srcPath.substr(0, srcPath.length() - 1);

    } else {
        _rootPath = srcPath;
    }

    // set parent path

    if (_rootPath == ZIP_PATH_SEPARATOR_STR) {
        // root "/"
        _parentPath = _rootPath;

    } else {
        int pos = _rootPath.rfind(ZIP_PATH_SEPARATOR_STR);
        _parentPath = _rootPath.substr(0, pos + 1);
    }

    // create new archive file

    rt = createArchive();

    if (-1 == rt) {
        fprintf(stderr, "create new archive %s failed\n",
                _archivePath.c_str());
        return -1;
    }

    // add root directory to archive
    rt = addDirToArchive(_rootPath);

    if (-1 == rt) {
        fprintf(stderr, "add %s to archive failed\n",
                _rootPath.c_str());

        rt = unchangeArchive();

        if (-1 == rt) {
            fprintf(stderr, "unchange archive failed\n");
        }

        rt = closeArchive();

        if (-1 == rt) {
            fprintf(stderr, "close archive failed\n");
        }

        return -1;
    }

    // close archive

    rt = closeArchive();

    if (-1 == rt) {
        fprintf(stderr, "close archive failed\n");
        return -1;
    }

    return 0;
}

int
Zip::uncompress(const std::string &destPath)
{
    int rt = 0;

    // must be an absolute path

    if (destPath.substr(0, 1) != ZIP_PATH_SEPARATOR_STR) {
        fprintf(stderr, "%s is not an absolute path\n",
                destPath.c_str());
        return -1;
    }

    // must be a directory

    struct stat st;

    rt = ::stat(destPath.c_str(), &st);

    if (-1 == rt) {
        fprintf(stderr, "path %s, stat() error, %s\n",
                destPath.c_str(), strerror(errno));
        return -1;
    }

    if (!S_ISDIR(st.st_mode)) {
        fprintf(stderr, "path %s is not a directory\n",
                destPath.c_str());
        return -1;
    }

    // set parent path

    if (destPath.substr(destPath.length() - 1) != ZIP_PATH_SEPARATOR_STR) {
        _parentPath = destPath + ZIP_PATH_SEPARATOR_STR;
    }

    // open archive

    rt = openArchive();

    if (-1 == rt) {
        fprintf(stderr, "open archive %s failed\n",
                _archivePath.c_str());
        return -1;
    }

    rt = ::zip_get_num_files(_pZip);

    if (-1 == rt) {
        fprintf(stderr, "zip_get_num_files() error, %s\n",
                ::zip_strerror(_pZip));
        return -1;
    }

    int files = rt;

    for (int i = 0; i < files; ++i) {
        rt = extractFile(i);

        if (-1 == rt) {
            fprintf(stderr, "extract file %d failed\n", i);
            return -1;
        }
    }

    return 0;
}

int
Zip::createArchive()
{
    int rt = 0;

    int error = 0;

    _pZip = ::zip_open(_archivePath.c_str(), ZIP_CREATE | ZIP_EXCL, &error);

    if (NULL == _pZip) {
        parseOpenError(error);
        return -1;
    }

    return 0;
}

int
Zip::openArchive()
{
    int rt = 0;

    int error = 0;

    _pZip = ::zip_open(_archivePath.c_str(), ZIP_CHECKCONS, &error);

    if (NULL == _pZip) {
        parseOpenError(error);
        return -1;
    }

    return 0;
}

int
Zip::closeArchive()
{
    int rt = 0;

    rt = ::zip_close(_pZip);

    if (0 == rt) {
        _pZip = NULL;
        return 0;
    }

    // error

    fprintf(stderr, "close archive %s failed, %s\n",
            _archivePath.c_str(), ::zip_strerror(_pZip));


    rt = unchangeArchive();

    if (-1 == rt) {
        fprintf(stderr, "unchange archive failed, %s\n",
                ::zip_strerror(_pZip));
    }

    rt = ::zip_close(_pZip);

    if (-1 == rt) {
        fprintf(stderr, "close archive %s failed, %s\n",
                _archivePath.c_str(), ::zip_strerror(_pZip));
    }

    _pZip = NULL;

    return -1;
}

int
Zip::unchangeArchive()
{
    int rt = 0;

    rt = ::zip_unchange_all(_pZip);

    if (-1 == rt) {
        fprintf(stderr, "unchange archive failed, %s\n",
                ::zip_strerror(_pZip));
        return -1;
    }

    return 0;
}

void
Zip::parseOpenError(int error)
{
    switch (error) {

    case ZIP_ER_EXISTS: {
            fprintf(stderr, "Path %s exists and ZIP_EXCL is set\n",
                    _archivePath.c_str());
            break;
        }

    case ZIP_ER_INCONS: {
            fprintf(stderr, "Inconsistencies were found in the file %s "
                    "and ZIP_CHECKCONS was specified\n",
                    _archivePath.c_str());
            break;
        }

    case ZIP_ER_INVAL: {
            fprintf(stderr, "Invalid archive path %s\n",
                    _archivePath.c_str());
            break;
        }

    case ZIP_ER_MEMORY: {
            fprintf(stderr, "Required memory could not be allocated\n");
            break;
        }

    case ZIP_ER_NOENT: {
            fprintf(stderr, "Path %s does not exist "
                    "and ZIP_CREATE is not set\n",
                    _archivePath.c_str());
            break;
        }

    case ZIP_ER_NOZIP: {
            fprintf(stderr, "Path %s is not a zip archive\n",
                    _archivePath.c_str());
            break;
        }

    case ZIP_ER_OPEN: {
            fprintf(stderr, "Path %s could not be opened\n",
                    _archivePath.c_str());
            break;
        }

    case ZIP_ER_READ: {
            fprintf(stderr, "A read error occurred, %s\n",
                    strerror(errno));
            break;
        }

    case ZIP_ER_SEEK: {
            fprintf(stderr, "Path %s does not allow seeks\n",
                    _archivePath.c_str());
            break;
        }

    default: {
            fprintf(stderr, "unexpected error on path %s\n",
                    _archivePath.c_str());
            break;
        }

    }
}

int
Zip::addDirToArchive(const std::string &path)
{
    int rt = 0;

    std::string entryName;
    std::string npath;

    // add current dir to archive

    if (path != ZIP_PATH_SEPARATOR_STR) {
        // not root "/"

        std::string dirName = path.substr(_parentPath.length());

        rt = ::zip_add_dir(_pZip, dirName.c_str());

        if (-1 == rt) {
            fprintf(stderr, "add %s to archive failed, %s\n",
                    path.c_str(), ::zip_strerror(_pZip));
            return -1;
        }
    }

    DIR *pDir = ::opendir(path.c_str());

    if (NULL == pDir) {
        fprintf(stderr, "path %s, opendir() error, %s\n",
                path.c_str(), strerror(errno));
        return -1;
    }

    // add its children

    struct dirent *pEnt = NULL;

    while (NULL != (pEnt = ::readdir(pDir))) {
        entryName = pEnt->d_name;

        // omit "." and ".."
        if (entryName == ZIP_DENTRY_CURRENT_DIR
            || entryName == ZIP_DENTRY_PARENT_DIR) {
            continue;
        }

        npath = path + ZIP_PATH_SEPARATOR_STR + entryName;

        if (DT_DIR == pEnt->d_type) {
            // directory, call myself to add it
            rt = addDirToArchive(npath);

            if (-1 == rt) {
                fprintf(stderr, "add %s to archive failed\n",
                        npath.c_str());
                ::closedir(pDir);
                return -1;
            }

        } else {
            // regular file, add it directly
            rt = addFileToArchive(npath);

            if (-1 == rt) {
                fprintf(stderr, "add %s to archive failed\n",
                        npath.c_str());
                ::closedir(pDir);
                return -1;
            }
        }
    }

    ::closedir(pDir);

    return 0;
}

int
Zip::addFileToArchive(const std::string &path)
{
    int rt = 0;

    struct zip_source *pSource = NULL;

    pSource = ::zip_source_file(_pZip, path.c_str(), 0, 0);

    if (NULL == pSource) {
        fprintf(stderr, "generate zip_source for %s failed, %s\n",
                path.c_str(), ::zip_strerror(_pZip));
        return -1;
    }

    std::string fileName = path.substr(_parentPath.length());

    rt = ::zip_add(_pZip, fileName.c_str(), pSource);

    if (-1 == rt) {
        fprintf(stderr, "add source for %s into archive failed, %s\n",
                path.c_str(), ::zip_strerror(_pZip));
        ::zip_source_free(pSource);
        pSource = NULL;
        return -1;
    }

    return 0;
}

int
Zip::extractFile(int idx)
{
    int rt = 0;

    // stat

    struct zip_stat st;

    rt = ::zip_stat_index(_pZip, idx, ZIP_FL_UNCHANGED, &st);

    if (-1 == rt) {
        fprintf(stderr, "zip_stat_index() error, %s\n",
                ::zip_strerror(_pZip));
        return -1;
    }

    std::string name = st.name;
    std::string path = _parentPath + name;

    // directory

    if (name.substr(name.length() - 1) == ZIP_PATH_SEPARATOR_STR) {
        rt = ::mkdir(path.c_str(), S_IRWXU);

        if (-1 == rt) {
            fprintf(stderr, "path %s, mkdir() error, %s\n",
                    path.c_str(), strerror(errno));
            return -1;
        }

        return 0;
    }

    // regular file

    uint32_t size = st.size;

    rt = ::open(path.c_str(), O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);

    if (-1 == rt) {
        fprintf(stderr, "path %s, open() error, %s\n",
                path.c_str(), strerror(errno));
        return -1;
    }

    int fd = rt;

    struct zip_file *pFile = NULL;

    pFile = ::zip_fopen_index(_pZip, idx, 0);

    if (NULL == pFile) {
        fprintf(stderr, "%s, zip_fopen_index() error, %s\n",
                name.c_str(), ::zip_strerror(_pZip));
        ::close(fd);
        return -1;
    }

    uint32_t totalRead = 0;
    uint32_t nread = 0;

    char buf[ZIP_BUFFER_SIZE];

    for (totalRead = 0; totalRead < size;) {
        memset(buf, 0, ZIP_BUFFER_SIZE);

        rt = ::zip_fread(pFile, buf, ZIP_BUFFER_SIZE);

        if (-1 == rt) {
            fprintf(stderr, "%s, zip_fread() error, %s\n",
                    name.c_str(), ::zip_file_strerror(pFile));
            ::close(fd);
            return -1;
        }

        nread = rt;

        rt = writen(fd, buf, nread);

        if (nread != rt) {
            fprintf(stderr, "%s, writen() error, %s\n",
                    name.c_str(), strerror(errno));
            ::close(fd);
            return -1;
        }

        totalRead += nread;

        if (nread < ZIP_BUFFER_SIZE) {
            break;
        }
    }

    ::close(fd);

    if (totalRead != size) {
        fprintf(stderr, "%s, extract file failed\n",
                name.c_str());
        return -1;
    }

    return 0;
}

ssize_t 
Zip::writen(int fd, const void *vptr, size_t n) 
{
    size_t nleft = 0;;
    ssize_t nwritten = 0;
    char *ptr = NULL;

    nleft = n;
    nwritten = 0;
    ptr = static_cast<char *>(const_cast<void *>(vptr));

    while (nleft > 0) {
        if ( (nwritten = write(fd, ptr, nleft)) < 0) {
            if (errno == EINTR) {
                nwritten = 0;
                continue;
            } else {
                fprintf(stderr, "Syscall Error: write. %s", strerror(errno));
                return -1;
            }
        } else if (nwritten == 0) {
            break;
        }

        nleft -= nwritten;
        ptr += nwritten;
    }

    return (n - nleft);
}

