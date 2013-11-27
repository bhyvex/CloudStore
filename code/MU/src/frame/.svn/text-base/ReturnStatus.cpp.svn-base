/*
 * @file ReturnStatus.cpp
 * @brief Return-value object for functions.
 * 
 * @version 1.0
 * @date Tue Jun 26 17:37:02 2012
 * 
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#include "ReturnStatus.h"

ReturnStatus::~ReturnStatus()
{

}

ReturnStatus::ReturnStatus() :
    returnValue(m_ReturnValue),
    errorCode(m_Code),
    infoCode(m_Code)
{
    m_ReturnValue = 0;
    m_Code = 0;
}

ReturnStatus::ReturnStatus(const long &rc, const long &code) :
    returnValue(m_ReturnValue),
    errorCode(m_Code),
    infoCode(m_Code)
{
    m_ReturnValue = rc;
    m_Code = code;
}

ReturnStatus::ReturnStatus(const ReturnStatus &status) :
    returnValue(m_ReturnValue),
    errorCode(m_Code),
    infoCode(m_Code)
{
    *this = status;
}

ReturnStatus&
ReturnStatus::operator=(const ReturnStatus &status)
{
    this->m_ReturnValue = status.m_ReturnValue;
    this->m_Code = status.m_Code;

    return *this;
}

bool
ReturnStatus::success()
{
    return m_ReturnValue >= 0 ? true : false;
}


