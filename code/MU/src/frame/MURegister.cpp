/*
 * @file MURegister.cpp
 * @brief 
 * 
 * @version 1.0
 * @date Mon Jul 23 01:52:09 2012
 * 
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#include "MURegister.h"

#include "MUWorkItemManager.h"

MURegister::~MURegister()
{

}

MURegister::MURegister()
{
    m_pCurrentItemManager = new MUWorkItemManager();
    m_pDelayItemManager = new MUWorkItemManager();

    m_BigLock = false;
}

