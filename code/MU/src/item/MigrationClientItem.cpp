/*
 * @file MigrationClientItem.cpp
 * @brief Async work for migration client.
 *
 * @version 1.0
 * @date Sun Jul 15 12:48:53 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#include "MigrationClientItem.h"

#include "frame/MUTask.h"
#include "frame/MUWorkItem.h"
#include "frame/MUMacros.h"
#include "frame/MUConfiguration.h"
#include "frame/ReturnStatus.h"
#include "dao/BucketDAO.h"

#include "log/log.h"

#include <string>

#include <unistd.h>

MigrationClientItem::~MigrationClientItem()
{
    if (0 != m_BucketDataFd) {
        ::close(m_BucketDataFd);
        m_BucketDataFd = 0;
    }
}

MigrationClientItem::MigrationClientItem(MUTask *pTask) :
    MUWorkItem(pTask)
{
    m_BucketDataFd = 0;
    m_BucketId = 0;
}

int
MigrationClientItem::process()
{
    switch (m_WorkType) {

    case WRITE_BUCKET_DATA: {
            writeBucketData();
            break;
        }

    case EXTRACT_BUCKET_DATA: {
            extractBucketData();
            break;
        }

    default: {
            DEBUG_LOG("Unexpected work type %d.", m_WorkType);

            m_ReturnStatus = ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
            break;
        }

    }

    return 0;
}

void
MigrationClientItem::writeBucketData()
{
    BucketDAO *pDAO = new BucketDAO();

    if (m_BucketDataFd <= 0) {
        m_ReturnStatus = pDAO->createMigrationTmpFile(
                             m_BucketId, &m_BucketDataFd);

        if (!m_ReturnStatus.success()) {
            ERROR_LOG("open migration temporary file failed.");
            delete pDAO;
            pDAO = NULL;

            return ;
        }
    }

    m_ReturnStatus = pDAO->writeMigrationTmpFile(
                         m_BucketDataFd, m_BucketData);

    delete pDAO;
    pDAO = NULL;
}

void
MigrationClientItem::extractBucketData()
{
    ::close(m_BucketDataFd);
    m_BucketDataFd = 0;

    BucketDAO *pDAO = new BucketDAO();

    m_ReturnStatus = pDAO->extractMigrationTmpFile(m_BucketId);

    if (!m_ReturnStatus.success()) {
        ERROR_LOG("extract migration temporary file failed.");
        delete pDAO;
        pDAO = NULL;

        return;
    }

    m_ReturnStatus = pDAO->deleteMigrationTmpFile(m_BucketId);

    if (!m_ReturnStatus.success()) {
        ERROR_LOG("delete migration temporary file failed.");
    }

    delete pDAO;
    pDAO = NULL;
}

