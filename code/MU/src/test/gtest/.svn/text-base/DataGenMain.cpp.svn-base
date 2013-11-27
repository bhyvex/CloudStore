/*
 * @file DataGenMain.cpp
 * @brief
 *
 * @version 1.0
 * @date Wed Aug  8 15:50:03 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#include "DataGen.h"

#include <stdlib.h>
#include <stdio.h>

#include <string>

#include "xml/Xml.h"
#include "log/log.h"

void usage(const std::string &cmd)
{
    printf("%s: {clean | gen}\n", cmd.c_str());
}

int
main(int argc, char *argv[])
{
    if (2 != argc) {
        usage(argv[0]);
        exit(1);
    }

    std::string cmd = argv[1];

    if (cmd != "clean" && cmd != "gen") {
        usage(argv[0]);
        exit(1);
    }

    int rt = 0;

    Xml *pXml = new Xml();

    xmlNode *pRoot = pXml->loadFile("TestConf.xml");

    if (NULL == pRoot) {
        ERROR_LOG("load configuration from TestConf.xml failed");
        exit(1);
    }

    std::string dataRoot;

    rt = pXml->getNodeValueByXPath("/TestConf/MU/FileSystemRoot", &dataRoot);

    if (-1 == rt) {
        ERROR_LOG("parse data root path from TestConf.xml failed");
        exit(1);
    }

    delete pXml;
    pXml = NULL;

    DataGen *pDataGen = new DataGen(dataRoot);

    if (cmd == "clean") {
        rt = pDataGen->clean();

        if (-1 == rt) {
            ERROR_LOG("clean data failed");
            exit(1);
        }

    } else {
        rt = pDataGen->gen();

        if (-1 == rt) {
            ERROR_LOG("gen data failed");
            exit(1);
        }
    }

    printf("success!\n");

    return 0;
}


