/*
 * @file BucketItem.cpp
 * @brief Bucket-related thread pool operations.
 *
 * @version 1.0
 * @date Wed Jul 11 15:34:00 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#include "BucketItem.h"

#include "frame/MUWorkItem.h"
#include "frame/MUTask.h"
#include "dao/BucketDAO.h"
#include "dao/LogDAO.h"

#include "log/log.h"

BucketItem::~BucketItem()
{

}

BucketItem::BucketItem(MUTask *pTask) :
    MUWorkItem(pTask)
{
    m_BucketId = 0;
}

int
BucketItem::process()
{
    switch (m_WorkType) {

    case DELETE_BUCKET: {
            deleteBucket();
            break;
        }

    case START_BUCKET:
                        {
                            startBucket();
                            break;
                        }

    default: {
            DEBUG_LOG("Unknown work type %d.", m_WorkType);
            break;
        }

    }

    return 0;
}


void
BucketItem::deleteBucket()
{
    BucketDAO *pDAO = new BucketDAO();

    m_ReturnStatus = pDAO->deleteBucket(m_BucketId);

    delete pDAO;
    pDAO = NULL;
}

void
BucketItem::startBucket()
{
    LogDAO *pDAO = new LogDAO();

    m_ReturnStatus = pDAO->queryCurrentBucketLogSeqNr(m_BucketId, &m_LogSeqNr);

    delete pDAO;
    pDAO = NULL;
}



