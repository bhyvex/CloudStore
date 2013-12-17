/*
 * @file Bucket.cpp
 * @brief To manage bucket state.
 * 
 * @version 1.0
 * @date Thu Jul  5 19:26:33 2012
 * 
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#include "Bucket.h"

#include <stdlib.h>

#include "frame/MUWorkItemManager.h"
#include "frame/MUMacros.h"
#include "protocol/MUMacros.h"

Bucket::~Bucket()
{
    delete m_pItemManager;
    m_pItemManager = NULL;
}

Bucket::Bucket()
{
    m_pItemManager = new MUWorkItemManager();
    
    m_BucketId = 0;
    m_BucketState = MU_BUCKET_START;
    m_LogSeq = 0;
}

