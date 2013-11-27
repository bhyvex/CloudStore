/*
 * =============================================================================
 * 
 *       Filename:  Exception.h
 * 
 *    Description:  异常封装
 * 
 *        Version:  1.0
 *        Created:  2011-05-26 19:07:08
 *  Last Modified:  2011-05-26 19:07:08
 *       Revision:  none
 *       Compiler:  gcc
 * 
 *         Author:  Shi Wei (sw), shiwei2012@gmail.com
 *        Company:  NDSL UESTC
 * 
 * =============================================================================
 */

#ifndef COMMON_SYS_EXCEPTION_H_
#define COMMON_SYS_EXCEPTION_H_

#include <exception>
#include <string>
#include <ostream>

/** 
* @brief Exception是对std::exception的封装 
*/
class Exception : public std::exception
{
public:

    Exception();

    Exception(const std::string &msg);

    Exception(const std::string &file, 
            int line);

    /**
     * @brief 构造函数
     *
     * @param file 抛出异常的源文件名
     * @param line 抛出异常的代码行
     */
    Exception(const std::string &msg,
            const std::string &file,
            int line);

    virtual ~Exception() throw();

    /**
     * @brief 获得异常类的名称
     *
     * @return 
     *      异常类的名称
     */
    virtual std::string name() const;

    /**
     * @brief 获得异常描述信息
     *
     * @return 
     *      异常描述信息
     */
    virtual const char* what() const throw();

    /**
     * @brief 获得抛出异常的源文件名
     *
     * @return 
     */
    std::string file() const;

    /**
     * @brief 获得抛出异常的代码行号
     *
     * @return 
     */
    int line() const;

    /**
     * @brief 获取异常信息
     *
     * @return 
     */
    std::string message() const;

protected:
    virtual void print(std::ostream &out) const;
    
private:
    
    static const std::string m_Name;  // name of this class
    mutable std::string m_WhatMessage;  // what信息
    std::string m_Message;  // 异常信息
    std::string m_File;  // source code file name
    int m_Line;  // source code line number
};

std::ostream& operator << (std::ostream& out, const Exception &ex);

/** 
 * @brief 空指针异常
 */
class NullPointerException : public Exception
{
public:
    
    NullPointerException();
    NullPointerException(const std::string &msg);
    NullPointerException(
            const std::string &file,
            int line);
    NullPointerException(const std::string &msg,
            const std::string &file,
            int line);
    virtual ~NullPointerException() throw();

    virtual std::string name() const;

private:
    static const std::string m_Name;
};

/** 
 * @brief 非法参数异常
 */
class IllegalArgumentException : public Exception
{
public:
    
    IllegalArgumentException();
    IllegalArgumentException(const std::string &msg);
    IllegalArgumentException(
            const std::string &file,
            int line);
    IllegalArgumentException(
            const std::string &msg,
            const std::string &file,
            int line);
    virtual ~IllegalArgumentException() throw();

    virtual std::string name() const;

private:

    static const std::string m_Name;
};

/** 
 * @brief 系统调用异常
 */
class SyscallException : public Exception
{
public:

    /**
     * @brief 
     *
     * @param error errno
     */
    SyscallException(int error = 0);
    SyscallException(const std::string &msg,
            int error = 0);
    SyscallException(const std::string &file,
            int line,
            int error = 0);
    SyscallException(const std::string &msg, 
            const std::string &file,
            int line,
            int error = 0);

    virtual std::string name() const;
    virtual int getErrorNo() const;

private:
    static const std::string m_Name;
    int m_Error;  // errno
};

#endif  // COMMON_SYS_EXCEPTION_H_

