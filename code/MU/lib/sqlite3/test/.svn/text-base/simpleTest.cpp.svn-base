/*
 * @file simpleTest.cpp
 * @brief A simple test for sqlite3 C/C++ interface.
 *
 * @version 1.0
 * @date Tue Jun 26 14:29:35 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#include <stdio.h>

#include <sqlite3.h>

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
    int i = 0;

    for (i = 0; i < argc; ++i) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }

    printf("\n");

    return 0;
}

int main(int argc, char *argv[])
{
    sqlite3 *pDb = NULL;

    char *zErrMsg = NULL;

    int rc = 0;

    if (3 != argc) {
        fprintf(stderr, "Usage: %s DATABASE SQL-STATEMENT\n", argv[0]);
        return 1;
    }

    rc = sqlite3_open(argv[1], &pDb);

    if (0 != rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(pDb));
        sqlite3_close(pDb);
        return 1;
    }

    rc = sqlite3_exec(pDb, argv[2], callback, 0, &zErrMsg);

    if (SQLITE_OK != rc) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }

    sqlite3_close(pDb);

    return 0;
}

