/*
 * @file MUTPRequest.h
 * @brief Thread pool request.
 * 
 * @version 1.0
 * @date Thu Jun 28 17:00:32 2012
 * 
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#ifndef MUTPRequest_H_
#define MUTPRequest_H_

class MUWorkItem;

struct MUTPRequest
{
    // concrete type of work item
    int m_ItemType;

    // work item
    MUWorkItem *m_pItem;

    virtual ~MUTPRequest();

    MUTPRequest();

    MUTPRequest(int itemType, MUWorkItem *pItem);
};

#endif  // MUTPRequest_H_

