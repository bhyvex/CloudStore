/*
 * @file CheckIdleUserItem.h
 * @brief  check and delete outdated user.
 *
 * @version 1.0
 * @date Tue Aug  7 16:29:25 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#ifndef ITEM_CHECKIDLEUSER_H_
#define ITEM_CHECKIDLEUSER_H_

#include "frame/MUWorkItem.h"

class CheckIdleUserItem : public MUWorkItem
{
public:
    enum _CheckIdleUserItemWorkType {
        CHECK_IDLE_USER,
    };

public:
    virtual ~CheckIdleUserItem();

    explicit CheckIdleUserItem(MUTask *pTask);

    int process();

private:
    void checkIdleUser();
};

#endif  // ITEM_CHECKIDLEUSER_H_

