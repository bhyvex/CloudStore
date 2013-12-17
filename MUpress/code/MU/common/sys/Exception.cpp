/*
 * =============================================================================
 * 
 *       Filename:  Exception.cpp
 * 
 *    Description:  异常封装
 * 
 *        Version:  1.0
 *        Created:  2011-05-26 19:10:26
 *  Last Modified:  2011-05-26 19:10:26
 *       Revision:  none
 *       Compiler:  gcc
 * 
 *         Author:  lpc, lvpengcheng6300@gmail.com
 *        Company:  NDSL UESTC
 * 
 * =============================================================================
 */

#include "Exception.h"

#include <string>
#include <ostream>
#include <sstream>

#include <cstdlib>

////////////////////////////////////////////////////////////////////////////////
// Exception - BEGIN -
////////////////////////////////////////////////////////////////////////////////

Exception::Exception() :
    m_Line(0)
{

}

Exception::Exception(const std::string &msg) :
    m_Message(msg)
{

}
    
Exception::Exception(
        const std::string &file,
        int line) :
    m_File(file),
    m_Line(line)
{

}

Exception::Exception(
        const std::string &msg,
        const std::string &file,
        int line) :
    m_Message(msg),
    m_File(file),
    m_Line(line)
{

}
    
Exception::~Exception() throw()
{
}

const std::string Exception::m_Name = "Exception";

void
Exception::print(std::ostream &out) const
{
    out << name() << " (IN FILE "
        << m_File << ", LINE "
        << m_Line << ") "
        << m_Message;
}

std::string 
Exception::name() const
{
    return m_Name;
}

const char* 
Exception::what() const throw()
{
    try {
        if (m_WhatMessage.empty()) {
            std::stringstream s;
            print(s);
            m_WhatMessage = s.str();
        }

        return m_WhatMessage.c_str();
    } catch (...) {

    }

    return "";
}

std::string 
Exception::file() const
{
    return m_File;
}

int 
Exception::line() const
{
    return m_Line;
}

std::string
Exception::message() const
{
    return m_Message;
}

std::ostream& 
operator << (std::ostream& out, const Exception& ex)
{
    out << ex.what();

    return out;
}

////////////////////////////////////////////////////////////////////////////////
// Exception - END -
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// NullPointerException - BEGIN -
////////////////////////////////////////////////////////////////////////////////

NullPointerException::NullPointerException() 
{

}

NullPointerException::NullPointerException(
        const std::string &msg) :
    Exception(msg)
{

}

NullPointerException::NullPointerException(
        const std::string &file,
        int line) :
    Exception(file, line)
{

}

NullPointerException::NullPointerException(
        const std::string &msg,
        const std::string &file,
        int line) :
    Exception(msg, file, line)
{

}

NullPointerException::~NullPointerException() throw()
{

}

const std::string NullPointerException::m_Name = "NullPointerException";

std::string 
NullPointerException::name() const
{
    return m_Name;
}

////////////////////////////////////////////////////////////////////////////////
// NullPointerException - END -
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// IllegalArgumentException - BEGIN -
////////////////////////////////////////////////////////////////////////////////

IllegalArgumentException::IllegalArgumentException()
{

}

IllegalArgumentException::IllegalArgumentException(
        const std::string &msg) :
    Exception(msg) 
{

}

IllegalArgumentException::IllegalArgumentException(
        const std::string &file,
        int line) :
    Exception(file, line)
{

}

IllegalArgumentException::IllegalArgumentException(
        const std::string &msg,
        const std::string &file, 
        int line) :
    Exception(msg, file, line)
{

}

IllegalArgumentException::~IllegalArgumentException() throw()
{
}

const std::string IllegalArgumentException::m_Name = "IllegalArgumentException";

std::string 
IllegalArgumentException::name() const
{
    return m_Name;
}

////////////////////////////////////////////////////////////////////////////////
// IllegalArgumentException - END -
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// SyscallException - BEGIN -
////////////////////////////////////////////////////////////////////////////////

SyscallException::SyscallException(int error) :
    m_Error(error)
{

}

SyscallException::SyscallException(
        const std::string &msg,
        int error) :
    Exception(msg),
    m_Error(error)
{

}

SyscallException::SyscallException(
        const std::string &file,
        int line,
        int error) :
    Exception(file, line),
    m_Error(error)
{

}

SyscallException::SyscallException(
        const std::string &msg,
        const std::string &file,
        int line,
        int error) :
    Exception(msg, file, line),
    m_Error(error)
{

}
    
const std::string SyscallException::m_Name = "SyscallException";

std::string 
SyscallException::name() const
{
    return m_Name;
}

int 
SyscallException::getErrorNo() const
{
    return m_Error;
}

////////////////////////////////////////////////////////////////////////////////
// SyscallException - END -
////////////////////////////////////////////////////////////////////////////////

