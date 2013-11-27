/*
 * @file Connection.h
 * @brief Database connection.
 *
 * @version 1.0
 * @date Mon Jul  9 09:28:40 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#ifndef Sqlite_Connection_H_
#define Sqlite_Connection_H_

#include "sqlite3/sqlite3.h"

#include <string>

class Statement;

class Connection
{
public:
    virtual ~Connection();

    Connection();

    /**
     * @brief open database file
     *
     * @param fileName
     * @param create create a new one if not exists?
     *
     * @return
     */
    int open(const std::string &fileName, bool create = true);

    void close();

    Statement *createStatement();

    /**
     * @brief begin a transaction
     *
     * @return 
     */
    int begin();

    /**
     * @brief commit transaction
     *
     * @return 
     */
    int commit();

    /**
     * @brief rollback transaction
     *
     * @return 
     */
    void rollback();

    /**
     * @brief have open transactions on this connection?
     *
     * @return 
     */
    bool inTransaction();

private:
    sqlite3 *m_pDb;
    bool m_bInTransaction;
};

class ConnectionGuard
{
public:
    virtual ~ConnectionGuard();

    explicit ConnectionGuard(Connection **ppConn);

private:
    Connection **_ppConn;
};

#endif  // Sqlite_Connection_H_

