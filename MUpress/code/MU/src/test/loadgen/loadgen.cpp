/*
 * @file loadgen.cpp
 * @brief load generator for MU
 *
 * @version 1.0
 * @date Mon Sep 17 10:00:52 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include <sstream>

#include "../tool/net.h"

#include "protocol/protocol.h"

int g_connfd = 0;
uint64_t g_uid = 0;
int g_blocks = 0;
int g_req_total = 0;
int g_req_current = 0;
int g_req_done = 0;
int g_req_error = 0;
int g_file_version = 0;

template <typename D, typename S>
D conv(S s)
{
    std::stringstream ss;
    D d;

    ss << s;
    ss >> d;

    return d;
}

void
sendPutFileReq()
{
    MsgHeader msg;
    msg.cmd = MSG_SYS_MU_PUT_FILE;
    msg.para1 = g_req_current;

    cstore::pb_MSG_SYS_MU_PUT_FILE putFile;

    putFile.set_uid(g_uid);
    putFile.set_token("silencemoon");
    putFile.set_path("/" + conv<std::string, int>(g_req_current++));

    cstore::File_Attr *attr = putFile.mutable_attr();;
    attr->set_version(g_file_version);
    attr->set_ctime(0);
    attr->set_mtime(0);
    attr->set_mode(0);
    attr->set_size(g_blocks * 512 * 1024);
    attr->set_type(MU_REGULAR_FILE);


    cstore::Block_Meta *block = NULL;

    for (int i = 0; i < g_blocks; ++i) {
        block = putFile.add_block_list();
        block->set_checksum(std::string(40, '1'));
    }

    std::string data;

    if (!putFile.SerializeToString(&data)) {
        fprintf(stderr, "protobuf serialize to string failed\n");
        exit(1);
    }

    msg.length = data.length();

    int rt = 0;

    rt = mu_test_tool_net::sendPacket(g_connfd, msg, data.c_str());

    if (-1 == rt) {
        fprintf(stderr, "send packet failed\n");
        exit(1);
    }
}

void
recvPutFileAck()
{
    InReq req;

    int rt = 0;

    rt = mu_test_tool_net::readPacket(g_connfd, &req);

    if (-1 == rt) {
        fprintf(stderr, "read packet failed\n");
        exit(1);
    }

    ++g_req_done;

    if (req.m_msgHeader.error != MU_OK) {
        fprintf(stderr, "mu error\n");
        ++g_req_error;
    }
}

int
main(int argc, char *argv[])
{
    if (8 != argc) {
        fprintf(stderr, "usage: %s MU_IP MU_PORT UID BLOCKS TOTAL_REQS "
                "CONN FILE_VERSION\n", argv[0]);
        exit(1);
    }

    g_uid = conv<uint64_t, std::string>(argv[3]);
    g_blocks = conv<int, std::string>(argv[4]);
    g_req_total = conv<int, std::string>(argv[5]);
    g_file_version = conv<int, std::string>(argv[7]);

    g_connfd = mu_test_tool_net::connectTo(argv[1], atoi(argv[2]));

    if (-1 == g_connfd) {
        fprintf(stderr, "connec to %s:%s failed\n", argv[1], argv[2]);
        exit(1);
    }

    for (; ;) {
        sendPutFileReq();

        recvPutFileAck();

        if (g_req_done == g_req_total) {
            break;
        }
    }

    return 0;
}

