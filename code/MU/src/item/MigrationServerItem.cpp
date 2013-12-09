/*
 * @file MigrationServerItem.cpp
 * @brief Async work for migration server.
 *
 * @version 1.0
 * @date Mon Jul 16 01:36:18 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#include "MigrationServerItem.h"

#include "frame/MUTask.h"
#include "frame/MUWorkItem.h"
#include "frame/MUMacros.h"
#include "frame/MUConfiguration.h"
#include "frame/ReturnStatus.h"
#include "dao/BucketDAO.h"

#include "log/log.h"

#include <string>

#include <unistd.h>

MigrationServerItem::~MigrationServerItem()
{
    if (0 != m_BucketDataFd) {
        ::close(m_BucketDataFd);
        m_BucketDataFd = 0;
    }
}

MigrationServerItem::MigrationServerItem(MUTask *pTask) :
    MUWorkItem(pTask)
{
    m_BucketDataFd = 0;
    m_MoreData = false;
}

int
MigrationServerItem::process()
{
    switch (m_WorkType) {

    case TAR_BUCKET_DATA: {
            tarBucketData();
            break;
        }

    case READ_BUCKET_DATA: {
            readBucketData();
            break;
        }

    default: {
            ERROR_LOG("Unexpected work type %d.", m_WorkType);

            m_ReturnStatus = ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
            break;
        }

    }

    return 0;
}

void
MigrationServerItem::readBucketData()
{
    BucketDAO *pDAO = new BucketDAO();

    if (m_BucketDataFd <= 0) {
        m_ReturnStatus = pDAO->openMigrationTmpFile(
                             m_BucketId, &m_BucketDataFd);

        if (!m_ReturnStatus.success()) {
            ERROR_LOG("open migration temporary file failed.");
            delete pDAO;
            pDAO = NULL;

            return ;
        }
    }

    m_ReturnStatus = pDAO->readMigrationTmpFile(
                         m_BucketDataFd, &m_BucketData);

    delete pDAO;
    pDAO = NULL;

    if (m_ReturnStatus.success()) {
        if (MU_MORE_DATA == m_ReturnStatus.infoCode) {
            m_MoreData = true;

        } else if (MU_NO_MORE_DATA == m_ReturnStatus.infoCode) {
            m_MoreData = false;
            ::close(m_BucketDataFd);
            m_BucketDataFd = 0;
        }

    } else {
        ERROR_LOG("read bucket data failed.");
    }
}

void
MigrationServerItem::tarBucketData()
{
    BucketDAO *pDAO = new BucketDAO();

    m_ReturnStatus = pDAO->tarMigrationTmpFile(m_BucketId);

    delete pDAO;
    pDAO = NULL;

    if (!m_ReturnStatus.success()) {
        ERROR_LOG("tar bucket data for migration failed.");
    }
}

