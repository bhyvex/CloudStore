/*
 * @file ReturnStatus.h
 * @brief Return-value object for functions.
 *
 * @version 1.0
 * @date Tue Jun 26 17:26:26 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#ifndef ReturnStatus_H_
#define ReturnStatus_H_

class ReturnStatus
{
public:
    virtual ~ReturnStatus();

    ReturnStatus();

    /**
     * @brief constructor
     *
     * @param rc return value
     * @param code error code or info code, depends on returnValue
     */
    ReturnStatus(const long &rc, const long &code = 0);

    ReturnStatus(const ReturnStatus &status);

    ReturnStatus &operator= (const ReturnStatus &status);

    /**
     * @brief current return value is a success or failure.
     *
     * @return true - if returnValue >= 0
     *         false - else
     */
    bool success();

public:
    const long &returnValue;
    const long &errorCode;
    const long &infoCode;

private:
    long m_ReturnValue;
    long m_Code;
};

#endif  // ReturnStatus_H_

