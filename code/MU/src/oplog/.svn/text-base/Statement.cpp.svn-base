/*
 * @file Statement.cpp
 * @brief Database statement.
 *
 * @version 1.0
 * @date Mon Jul  9 14:10:48 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#include "Statement.h"

#include "sqlite3/sqlite3.h"

#include "log/log.h"

#include <string>

#include <inttypes.h>

Statement::~Statement()
{
    int rt = 0;

    if (NULL != m_pStmt) {
        rt = sqlite3_finalize(m_pStmt);

        if (SQLITE_OK != rt) {
            DEBUG_LOG("sqlite3_finalize() error, %s.", sqlite3_errmsg(m_pDb));
        }

        m_pStmt = NULL;
    }

    m_pDb = NULL;
}

Statement::Statement(sqlite3 *pDb)
{
    m_pDb = pDb;
}

int
Statement::prepare(const std::string &sql)
{
    int rt = 0;

    rt = sqlite3_prepare(
             m_pDb,
             sql.c_str(),
             sql.length(),
             &m_pStmt,
             NULL
         );

    if (SQLITE_OK != rt) {
        DEBUG_LOG("sqlite3_prepare() error, %s.", sqlite3_errmsg(m_pDb));
        return -1;
    }

    return 0;
}

int
Statement::bindInt64(int idx, int64_t val)
{
    int rt = 0;

    rt = sqlite3_bind_int64(m_pStmt, idx, val);

    if (SQLITE_OK != rt) {
        DEBUG_LOG("sqlite3_bind_int64() error, %s.", sqlite3_errmsg(m_pDb));
        return -1;
    }

    return 0;
}

int
Statement::bindText(int idx, const std::string &val)
{
    int rt = 0;

    rt = sqlite3_bind_text(
             m_pStmt,
             idx,
             val.c_str(),
             val.length(),
             NULL
         );

    if (SQLITE_OK != rt) {
        DEBUG_LOG("sqlite3_bind_text() error, %s.", sqlite3_errmsg(m_pDb));
        return -1;
    }

    return 0;
}

int
Statement::step()
{
    int rt = 0;

    rt = sqlite3_step(m_pStmt);

    if (SQLITE_DONE == rt) {
        return 0;

    } else if (SQLITE_ROW == rt) {
        return SQLITE_ROW;

    } else {
        DEBUG_LOG("sqlite3_step() error, error code %d, %s.",
                  rt,
                  sqlite3_errmsg(m_pDb));
        return -1;
    }

    return 0;  // make compiler happy
}

int
Statement::columnBytes(int idx)
{
    int rt = 0;

    rt = sqlite3_column_bytes(m_pStmt, idx);

    if (rt < 0) {
        DEBUG_LOG("sqlite3_column_bytes() error, %s.", sqlite3_errmsg(m_pDb));
        return -1;
    }

    return rt;
}

int64_t
Statement::columnInt64(int idx)
{
    return  sqlite3_column_int64(m_pStmt, idx);
}

const char *
Statement::columnText(int idx)
{
    return reinterpret_cast<const char *>(
               sqlite3_column_text(m_pStmt, idx)
           );
}

void
Statement::close()
{
    delete this;
}


int
Statement::clearBindings()
{
    int rt = 0;

    rt = sqlite3_clear_bindings(m_pStmt);

    if (SQLITE_OK != rt) {
        DEBUG_LOG("sqlite3_clear_bindings() error, %s.", sqlite3_errmsg(m_pDb));
        return -1;
    }

    return 0;
}

int
Statement::reset()
{
    int rt = 0;

    rt = sqlite3_reset(m_pStmt);

    if (SQLITE_OK != rt) {
        DEBUG_LOG("sqlite3_reset() error, %s.", sqlite3_errmsg(m_pDb));
        return -1;
    }

    return 0;
}

int
Statement::clean()
{
    int rt = 0;

    rt = reset();

    if (-1 == rt) {
        DEBUG_LOG("reset() error");
        return -1;
    }

    rt = clearBindings();

    if (-1 == rt) {
        DEBUG_LOG("clearBindings() error");
        return -1;
    }

    return 0;
}

int
Statement::execute()
{
    return step();
}

int 
Statement::execute(const std::string &sql)
{
    int rt = 0;

    rt = prepare(sql);
    if (-1 == rt) {
        ERROR_LOG("prepare sql failed, %s", sql.c_str());
        return -1;
    }

    rt = step();
    if (-1 == rt) {
        ERROR_LOG("execute sql failed, %s", sql.c_str());
        return -1;
    }

    return 0;
}

StatementGuard::~StatementGuard()
{
    (*_ppStmt)->close();
    (*_ppStmt) = NULL;
}

StatementGuard::StatementGuard(Statement **ppStmt)
{
    _ppStmt = ppStmt;
}


