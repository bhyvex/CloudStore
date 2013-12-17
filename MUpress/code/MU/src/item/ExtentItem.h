/*
 * @file ExtentItem.h
 * @brief Async operations when extending bucket
 *
 * @version 1.0
 * @date Mon Jul 16 11:49:49 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#ifndef item_ExtentItem_H_
#define item_ExtentItem_H_

#include <inttypes.h>

#include <list>

#include "frame/MUWorkItem.h"
#include "frame/ReturnStatus.h"

class ExtentItem : public MUWorkItem
{
public:
    enum _ExtentItemWorkType {
        EXTENT_BUCKET,
    };

public:
    virtual ~ExtentItem();

    explicit ExtentItem(MUTask *pTask);

    int process();

    inline void setNewModNr(uint64_t modNr);

    inline void setLastModNr(uint64_t modNr);

    inline const std::list<uint64_t> &getNewBucketList();

private:

    void extent();

    ReturnStatus doExtent(uint64_t nBucketId);

    ReturnStatus fallback(uint64_t nBucketId);

private:
    uint64_t m_NewModNr;
    uint64_t m_LastModNr;
    std::list<uint64_t> m_NewBucketList;

};

void
ExtentItem::setNewModNr(uint64_t modNr)
{
    m_NewModNr = modNr;
}

void
ExtentItem::setLastModNr(uint64_t modNr)
{
    m_LastModNr = modNr;
}

const std::list<uint64_t> &
ExtentItem::getNewBucketList()
{
    return m_NewBucketList;
}

#endif  // item_ExtentItem_H_

