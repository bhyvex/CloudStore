/*
 * @file main.cpp
 * @brief main .... what can I say about it ....
 *
 * @version 1.0
 * @date Mon Jul  2 20:53:36 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#include <string>
#include <iostream>

#include <signal.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>

#include "MUConfiguration.h"
#include "RunControl.h"

static void sighandler(int sig_no)
{
    exit(0);
}

void usage(const std::string &exeName)
{
    fprintf(stderr, "Usage: %s [-c configFilePath]\n", exeName.c_str());
}

int main(int argc, char *argv[])
{
    signal(SIGINT, sighandler);
    signal(SIGUSR1, sighandler);

    if (1 == argc) {
        usage(argv[0]);
        exit(1);
    }

    int rt = 0;

    std::string configFileName;

    // parse arguments
    int opt = 0;

    while (-1 != (opt = getopt(argc, argv, "c:"))) {
        switch (opt) {
        case 'c':
            configFileName = optarg;
            break;

        default:
            usage(argv[0]);
            exit(1);
            break;
        }
    }

    rt = MUConfiguration::getInstance()->configWithXML(configFileName);

    if (-1 == rt) {
        fprintf(stderr, "Load configuration from file %s failed. Program will exit.",
                  configFileName.c_str());
        exit(1);
    }

    RunControl::getInstance()->initialize();

    RunControl::getInstance()->run();

    return 0;
}

