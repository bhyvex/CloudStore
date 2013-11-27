/*
 * @file Connection.cpp
 * @brief Database connection.
 *
 * @version 1.0
 * @date Mon Jul  9 10:42:39 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#include "Connection.h"

#include "sqlite3/sqlite3.h"

#include "Statement.h"

#include "log/log.h"

#include <string>

Connection::~Connection()
{
    if (NULL != m_pDb) {
        close();
    }
}

Connection::Connection()
{
    m_pDb = NULL;
    m_bInTransaction = false;
}

int
Connection::open(const std::string &fileName, bool create)
{
    int rt = 0;

    if (create) {
        rt = sqlite3_open_v2(fileName.c_str(), &m_pDb,
                             SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
                             NULL);

        if (SQLITE_OK != rt) {
            DEBUG_LOG("sqlite3_open() error, %s", sqlite3_errmsg(m_pDb));
            return -1;
        }

    } else {
        rt = sqlite3_open_v2(fileName.c_str(), &m_pDb,
                             SQLITE_OPEN_READWRITE,
                             NULL);

        if (SQLITE_OK != rt) {
            DEBUG_LOG("sqlite3_open() error, %s", sqlite3_errmsg(m_pDb));
            return -1;
        }
    }

    return 0;
}

void
Connection::close()
{
    int rt = 0;

    if (NULL != m_pDb) {
        rt = sqlite3_close(m_pDb);

        if (SQLITE_OK != rt) {
            DEBUG_LOG("sqlite3_close error, %s.", sqlite3_errmsg(m_pDb));
        }

        m_pDb = NULL;
    }
}

Statement *
Connection::createStatement()
{
    return new Statement(m_pDb);
}

ConnectionGuard::~ConnectionGuard()
{
    (*_ppConn)->close();
    delete(*_ppConn);
    (*_ppConn) = NULL;
}

ConnectionGuard::ConnectionGuard(Connection **ppConn)
{
    _ppConn = ppConn;
}

int
Connection::begin()
{
    int rt = 0;

    Statement *pStmt = createStatement();

    if (NULL == pStmt) {
        ERROR_LOG("get database statement failed");
        return -1;
    }

    StatementGuard stmtGuard(&pStmt);

    std::string sql = "begin transaction";

    rt = pStmt->execute(sql);

    if (-1 == rt) {
        ERROR_LOG("execute sql failed, %s", sql.c_str());
        return -1;
    }

    m_bInTransaction = true;

    return 0;
}

int
Connection::commit()
{
    if (!m_bInTransaction) {
        WARN_LOG("There's no open transactions on current connection.");
        return 0;
    }

    int rt = 0;

    Statement *pStmt = createStatement();

    if (NULL == pStmt) {
        ERROR_LOG("get database statement failed");
        return -1;
    }

    StatementGuard stmtGuard(&pStmt);

    std::string sql = "commit transaction";

    rt = pStmt->execute(sql);

    if (-1 == rt) {
        ERROR_LOG("execute sql failed, %s", sql.c_str());
        return -1;
    }

    m_bInTransaction = false;

    return 0;
}

void
Connection::rollback()
{
    if (!m_bInTransaction) {
        WARN_LOG("There's no open transactions on current connection.");
        return ;
    }

    int rt = 0;

    Statement *pStmt = createStatement();

    if (NULL == pStmt) {
        ERROR_LOG("get database statement failed");
        return ;
    }

    StatementGuard stmtGuard(&pStmt);

    std::string sql = "rollback transaction";

    rt = pStmt->execute(sql);

    if (-1 == rt) {
        ERROR_LOG("execute sql failed, %s", sql.c_str());

        return ;
    }

    m_bInTransaction = false;
}

bool
Connection::inTransaction()
{
    return m_bInTransaction;
}


