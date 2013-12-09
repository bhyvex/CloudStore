/*
 * @file ExtentItem.cpp
 * @brief Async operations when extending bucket
 *
 * @version 1.0
 * @date Tue Jul 17 15:20:18 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#include "ExtentItem.h"

#include <math.h>

#include <list>
#include <memory>

#include "frame/MUWorkItem.h"
#include "frame/ReturnStatus.h"
#include "dao/BucketDAO.h"

ExtentItem::~ExtentItem()
{

}

ExtentItem::ExtentItem(MUTask *pTask) :
    MUWorkItem(pTask)
{
    m_NewModNr = 0;
    m_LastModNr = 0;
}

int
ExtentItem::process()
{
    switch (m_WorkType) {

    case EXTENT_BUCKET: {
            extent();

            break;
        }

    default: {
            ERROR_LOG("Unknown work type %d.", m_WorkType);

            m_ReturnStatus = ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);

            break;
        }

    }

    return 0;
}

void
ExtentItem::extent()
{
    int modDiff = m_NewModNr - m_LastModNr;
    int newBuckets = ::pow(2, modDiff) - 1;
    uint64_t lastTotalBuckets = ::pow(2, m_LastModNr);

    uint64_t nBucketId = 0;

    for (int i = 0; i < newBuckets; ++i) {
        nBucketId = m_BucketId + lastTotalBuckets * (i + 1);
        m_ReturnStatus = doExtent(nBucketId);

        if (!m_ReturnStatus.success()) {
            ERROR_LOG("split bucket %llu from bucket %llu failed",
                      nBucketId, m_BucketId);

            break;
        }

        m_NewBucketList.push_back(nBucketId);
    }

    if (!m_ReturnStatus.success()) {
        // extent error, fallback
        INFO_LOG("extent bucket %llu failed, try to fallback");

        for (std::list<uint64_t>::iterator it = m_NewBucketList.begin();
             it != m_NewBucketList.end(); ++it) {
            fallback(*it);
            INFO_LOG("fallback new bucket %llu", *it);
        }
    }
}

ReturnStatus
ExtentItem::doExtent(uint64_t nBucketId)
{
    ReturnStatus rs;

    std::auto_ptr<BucketDAO> pDAO = std::auto_ptr<BucketDAO>(
                                        new BucketDAO());

    // get all users in bukcet

    std::list<uint64_t> userList;

    rs = pDAO->getAllUsersInBucket(m_BucketId, &userList);

    if (!rs.success()) {
        ERROR_LOG("Get user list from bucket %ld failed.", m_BucketId);
        return rs;
    }

    // create new bucket
    //rs = pDAO->createBucket(nBucketId);
    rs = pDAO->createBucketIfNotExist(nBucketId);

    if (!rs.success()) {
        ERROR_LOG("Create new bucket %ld failed.", nBucketId);
        return rs;
    }

    // move users

    uint64_t newTotalBuckets = ::pow(2, m_NewModNr);

    for (std::list<uint64_t>::iterator it = userList.begin();
         it != userList.end(); ++it) {
        if (*it % newTotalBuckets == nBucketId) {
            rs = pDAO->moveUser(*it, m_BucketId, nBucketId);

            if (!rs.success()) {
                ERROR_LOG("Move user from bukcet %ld to bucket %ld failed.",
                          m_BucketId, nBucketId);
            }
        }
    }

    // link log file

    rs = pDAO->linkLogFile(m_BucketId, nBucketId);

    if (!rs.success()) {
        ERROR_LOG("Link log file from bucket %ld to bucket %ld failed.",
                  m_BucketId, nBucketId);

        return rs;
    }

    return ReturnStatus(MU_SUCCESS);
}

ReturnStatus
ExtentItem::fallback(uint64_t nBucketId)
{
    ReturnStatus rs;

    std::auto_ptr<BucketDAO> pDAO = std::auto_ptr<BucketDAO>(
                                        new BucketDAO());

    // get all users in new bukcet

    std::list<uint64_t> userList;

    rs = pDAO->getAllUsersInBucket(nBucketId, &userList);

    if (!rs.success()) {
        ERROR_LOG("Get user list from bucket %ld failed.", nBucketId);
        return rs;
    }

    // move users back to original bucket

    for (std::list<uint64_t>::iterator it = userList.begin();
         it != userList.end(); ++it) {
        rs = pDAO->moveUser(*it, nBucketId, m_BucketId);

        if (!rs.success()) {
            ERROR_LOG("Move user from new bukcet %llu to "
                      "original bucket %llu failed.",
                      nBucketId, m_BucketId);
        }
    }

    // remove new bucket

    rs = pDAO->deleteBucket(nBucketId);

    if (!rs.success()) {
        ERROR_LOG("delete new bucket %llu failed");
        return rs;
    }

    return ReturnStatus(MU_SUCCESS);
}


