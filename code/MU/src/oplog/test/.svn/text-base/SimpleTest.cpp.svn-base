/*
 * @file SimpleTest.cpp
 * @brief
 *
 * @version 1.0
 * @date Mon Jul  9 14:47:59 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#include "oplog/Connection.h"
#include "oplog/Statement.h"

#include <string>
#include <iostream>

int main()
{
    Connection *pConn = new Connection();
    pConn->open("SimpleTest.db");

    Statement *pStmt = pConn->createStatement();

    pStmt->prepare("create table t(id bigint, val varchar(50))");

    pStmt->execute();

    //pStmt->close();

    //pStmt = pConn->createStatement();
    
    pStmt->clear();

    pStmt->prepare("insert into t values(?, ?)");

    pStmt->bindInt64(1, 1023);

    std::string val = "施伟";
    pStmt->bindText(2, val);

    pStmt->execute();

    //pStmt->close();

    //pStmt = pConn->createStatement();

    pStmt->clear();

    pStmt->prepare("select * from t");

    pStmt->execute();

    std::cout << pStmt->columnInt64(0) << std::endl;
    std::cout << pStmt->columnText(1) << std::endl;

    return 0;
}
