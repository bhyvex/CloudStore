/*
 * @file InsertTest.cpp
 * @brief Test insertion time of sqlite3.
 *
 * @version 1.0
 * @date Thu Sep  6 15:25:08 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#include <sys/time.h>
#include <stdio.h>

#include "oplog/Connection.h"
#include "oplog/Statement.h"

int
main(int argc, char *argv[])
{
    int rt = 0;

    Connection *pConn = new Connection();

    rt = pConn->open("InsertTest.db", true);

    if (-1 == rt) {
        fprintf(stderr, "open database failed\n");
        return -1;
    }

    Statement *pStmt = pConn->createStatement();

    rt = pStmt->prepare("create table test (id bigint primary key, "
                        "value bigint)");

    if (-1 == rt) {
        fprintf(stderr, "prepare sql failed\n");
        return -1;
    }

    rt = pStmt->execute();

    if (-1 == rt) {
        fprintf(stderr, "execute sql failed\n");
        return -1;
    }

    pStmt->close();

    pStmt = pConn->createStatement();

    rt = pStmt->prepare("insert into test (id, value) values (?, ?)");

    if (-1 == rt) {
        fprintf(stderr, "prepare sql failed\n");
        return -1;
    }

    rt = pStmt->bindInt64(1, 1);

    if (-1 == rt) {
        fprintf(stderr, "bind error, index 1\n");
        return -1;
    }

    rt = pStmt->bindInt64(2, 2);

    if (-1 == rt) {
        fprintf(stderr, "bind error, index 2\n");
        return -1;
    }

    int records = 0;

    struct timeval begin;
    struct timeval end;

    gettimeofday(&begin, NULL);

    records = 100;

    for (int i = 0; i < records; ++i) {
        rt = pStmt->execute();

        if (-1 == rt) {
            fprintf(stderr, "execute error\n");
            return -1;
        }

        rt = pStmt->clean();

        if (-1 == rt) {
            fprintf(stderr, "reset error\n");
            return -1;
        }
    }

    gettimeofday(&end, NULL);

    int msec = (end.tv_sec * 1000 + end.tv_usec / 1000) -
               (begin.tv_sec * 1000 + begin.tv_usec / 1000);

    fprintf(stderr, "records per second: %d\n", records * 1000 / msec);

    rt = pConn->begin();

    if (-1 == rt) {
        fprintf(stderr, "begin transaction failed\n");
        return -1;
    }

    gettimeofday(&begin, NULL);

    records = 100000;

    for (int i = 0; i < records; ++i) {
        rt = pStmt->execute();

        if (-1 == rt) {
            fprintf(stderr, "execute error\n");
            return -1;
        }

        rt = pStmt->clean();

        if (-1 == rt) {
            fprintf(stderr, "reset error\n");
            return -1;
        }

        if (i % 5000 == 0) {
            rt = pConn->commit();

            if (-1 == rt) {
                fprintf(stderr, "commit transaction failed\n");
                return -1;
            }

            rt = pConn->begin();

            if (-1 == rt) {
                fprintf(stderr, "begin transaction failed\n");
                return -1;
            }
        }
    }

    rt = pConn->commit();

    if (-1 == rt) {
        fprintf(stderr, "commit transaction failed\n");
        return -1;
    }

    gettimeofday(&end, NULL);

    msec = (end.tv_sec * 1000 + end.tv_usec / 1000) -
           (begin.tv_sec * 1000 + begin.tv_usec / 1000);

    fprintf(stderr, "transaction records per second: %d\n",
            records * 1000 / msec);

    return 0;
}


