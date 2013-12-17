/*
 * @file Statement.h
 * @brief Database statement.
 *
 * @version 1.0
 * @date Mon Jul  9 10:50:02 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#ifndef Sqlite3_Statement_H_
#define Sqlite3_Statement_H_

#include "sqlite3/sqlite3.h"

#include <string>

#include <inttypes.h>

class Connection;

class Statement
{
    friend class Connection;

public:
    int prepare(const std::string &sql);

    int bindInt64(int idx, int64_t val);

    int bindText(int idx, const std::string &val);

    int execute();

    int execute(const std::string &sql);

    int step();

    int columnBytes(int idx);

    int64_t columnInt64(int idx);

    const char *columnText(int idx);

    int clearBindings();

    int reset();

    int clean();

    void close();

protected:
    virtual ~Statement();

    Statement(sqlite3 *pDb);

private:
    sqlite3 *m_pDb;
    sqlite3_stmt *m_pStmt;
};

class StatementGuard
{
public:
    virtual ~StatementGuard();

    explicit StatementGuard(Statement **ppStmt);

private:
    Statement **_ppStmt;
};


#endif  // Sqlite3_Statement_H_

