/*
 * @file Zip.h
 * @brief create a zip archive or uncompress a zip archive.
 *
 * @version 1.0
 * @date Wed Aug 15 20:55:48 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#ifndef MU_ZIP_ZIP_H_
#define MU_ZIP_ZIP_H_

#include <string>

#include <zip.h>

class Zip
{
public:
    virtual ~Zip();

    explicit Zip(const std::string &path);

    int compress(const std::string &srcPath);

    int uncompress(const std::string &destPath);

private:
    ssize_t writen(int fd, const void *vptr, size_t n);

    void parseOpenError(int error);

    int createArchive();

    int openArchive();

    int closeArchive();

    int unchangeArchive();

    int addToArchive(const std::string &path);

    int addDirToArchive(const std::string &path);

    int addFileToArchive(const std::string &path);

    int extractFile(int idx);

private:
    // zip archive struct
    struct zip *_pZip;
    // zip archive file path
    std::string _archivePath;
    // root dir to compress
    std::string _rootPath;
    // parent dir of _rootPath or dir which extract archive into
    std::string _parentPath;

};

#endif  // MU_ZIP_ZIP_H_

