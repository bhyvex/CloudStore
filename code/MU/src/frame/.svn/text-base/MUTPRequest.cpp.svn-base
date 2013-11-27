/*
 * @file MUTPRequest.cpp
 * @brief Thread pool request.
 * 
 * @version 1.0
 * @date Thu Jun 28 17:03:07 2012
 * 
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#include "MUTPRequest.h"

#include "MUWorkItem.h"

MUTPRequest::~MUTPRequest()
{
    m_pItem = NULL;
}

MUTPRequest::MUTPRequest()
{
    m_ItemType = 0;
    m_pItem = NULL;
}

MUTPRequest::MUTPRequest(int itemType, MUWorkItem *pItem)
{
    m_ItemType = itemType;
    m_pItem = pItem;
}
