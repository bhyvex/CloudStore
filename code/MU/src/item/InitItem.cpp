/*
 * @file InitItem.cpp
 * @brief Init system.
 *
 * @version 1.0
 * @date Fri Jul 20 15:19:08 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#include "InitItem.h"

#include "dao/BucketDAO.h"
#include "oplog/LogAccessEngine.h"

InitItem::~InitItem()
{

}

InitItem::InitItem(MUTask *pTask) :
    MUWorkItem(pTask)
{

}

int
InitItem::process()
{
    switch (m_WorkType) {

    case CREATE_BUCKETS: {
            createBuckets();

            break;
        }

    default: {
            DEBUG_LOG("unexpected work type %d.", m_WorkType);
            m_ReturnStatus = ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);

            break;
        }
    }

    return 0;
}

void
InitItem::createBuckets()
{
    ReturnStatus rs;
    bool failed = false;

    BucketDAO *pDAO = new BucketDAO();

    for (std::list<uint64_t>::iterator it = m_BucketList.begin();
         it != m_BucketList.end(); ++it) {
        rs = pDAO->createBucketIfNotExist(*it);

        if (!rs.success()) {
            ERROR_LOG("create bucket %" PRIu64 " failed.", *it);
            failed = true;
        }

        rs = LogAccessEngine::getInstance()->createLogFile(*it);

        if (!rs.success()) {
            ERROR_LOG("create log file for bucket %" PRIu64 " failed", *it);
            failed = true;
        }
    }

    delete pDAO;
    pDAO = NULL;

    if (failed) {
        m_ReturnStatus = ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);

    } else {
        m_ReturnStatus = ReturnStatus(MU_SUCCESS);
    }
}



