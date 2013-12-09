/*
 * @file LogRotationCheckItem.cpp
 * @brief Check current status to decide whether log rotations should be done.
 *
 * @version 1.0
 * @date Tue Jul 10 14:00:31 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#include "LogRotationCheckItem.h"

#include "frame/MUWorkItem.h"
#include "frame/MUTask.h"
#include "frame/MUConfiguration.h"
#include "frame/MUMacros.h"
#include "dao/LogFileDAO.h"

#include "log/log.h"
#include "util/util.h"

LogRotationCheckItem::~LogRotationCheckItem()
{

}

LogRotationCheckItem::LogRotationCheckItem(MUTask *pTask) :
    MUWorkItem(pTask)
{

}

int
LogRotationCheckItem::process()
{
    switch (m_WorkType) {

    case CHECK_LOG_FILE_SIZE: {
            checkLogFileSize();
            break;
        }

    default: {
            ERROR_LOG("Unknown work type %d.", m_WorkType);
            break;
        }

    }

    return 0;
}

void
LogRotationCheckItem::checkLogFileSize()
{
    LogFileDAO *pDAO = new LogFileDAO();

    m_ReturnStatus = pDAO->checkLogFileSize(
                         m_BucketList, &m_LogMap);

    delete pDAO;
    pDAO = NULL;
}


