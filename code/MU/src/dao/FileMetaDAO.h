/*
 * @file FileMetaDAO.h
 * @brief File metadata access object.
 *
 * @version 1.0
 * @date Wed Jul  4 10:36:25 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#ifndef FileMetaDAO_H_
#define FileMetaDAO_H_

#include "frame/MUDAO.h"
#include "frame/ReturnStatus.h"
#include "data/FileMeta.h"

#include <string>
#include <inttypes.h>



class Args;
class FileMetaDAO : public MUDAO
{
public:
    virtual ~FileMetaDAO();

    FileMetaDAO();

    bool setBucketID(uint64_t Id);
    bool setUserID(uint64_t Id);

    ReturnStatus putDir(const std::string &path);

    ReturnStatus delDir(const std::string &path, uint64_t *pDelta);

    ReturnStatus getDir(const std::string &path,
                        std::list<PDEntry> *pEntryList);

    ReturnStatus statDir(const std::string &path, FileMeta *pMeta);

    ReturnStatus getDir2(const std::string &path,
                         std::list<EDEntry> *pEntryList);

    ReturnStatus movDir(const std::string &srcPath,
                        const std::string &destPath);

    ReturnStatus putFile(const std::string &path,
                         const FileMeta &meta,
                         FileMeta *pMeta,
                         int *pDelta);

    ReturnStatus delFile(const std::string &path, int *pDelta);

    ReturnStatus getFile(const std::string &path, FileMeta *pMeta);

    ReturnStatus movFile(const std::string &srcPath,
                         const std::string &destPath);

protected:
    /**
     * @brief Get the prefix of path. i.e. prefix of /a/b/c is /a/b/
     *
     * @param path
     *
     * @return prefix
     */
    std::string prefix(const std::string &path);

    /**
     * @brief check the prefix to indicate errors occurred on path
     *
     * @param path full path
     *
     * @return
     */
    ReturnStatus checkPrefix(const std::string &path);

    /**
     * @brief Delete a specified directory, even it's no empty.
     *
     * @param path
     * @param pDelta [out] changed user quota
     *
     * @return
     */
    ReturnStatus rmdirRecursive(const std::string &path, uint64_t *pDelta);

    /**
     * @brief Add file size to pDelta
     *
     * @param path
     * @param pDelta [out]
     */
    void addFileSizeToDelta(const std::string &path, uint64_t *pDelta);

    /**
     * @brief path refers to a direcotry?
     *
     * @param path
     *
     * @return
     */
    ReturnStatus isdir(const std::string &path);

    /**
     * @brief path refers to a regular file?
     *
     * @param path
     *
     * @return
     */
    ReturnStatus isfile(const std::string &path);

    /**
     * @brief create a new file
     *
     * @param path
     * @param meta
     * @param pMeta [out]
     * @param pDelta [out] user quota delta
     *
     * @return
     */
    ReturnStatus createFile(
        const std::string &path,
        const FileMeta &meta,
        FileMeta *pMeta,
        int *pDelta);

    /**
     * @brief update an existed file
     *
     * @param path
     * @param meta meta to update
     * @param pMeta return current file version via this argument
     *          when VERSION_OUTDATED error occurs
     * @param pDelta [out] user quota delta
     *
     * @return
     */
    ReturnStatus updateFile(const std::string &path, const FileMeta &meta,
                            FileMeta *pMeta, int *pDelta);

    /**
     * @brief write file metadata to file specified by fd.
     *
     * @param fd
     * @param meta
     *
     * @return
     */
    ReturnStatus writeFileMeta(Args *fd, const FileMeta &meta);

    /**
     * @brief Read file metadata from specified file.
     *
     * @param fd
     * @param pMeta [out]
     *
     * @return
     */
    ReturnStatus readFileMeta(Args *fd, FileMeta *pMeta);

	uint64_t m_BucketId; 
	uint64_t m_UserId;

};

#endif  // FileMetaDAO_H_

