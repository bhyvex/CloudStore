/*
 * @file ClientTask.cpp
 * @brief
 *
 * @version 1.0
 * @date Mon Jul  2 19:58:10 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#include "ClientTask.h"

#include <assert.h>

#include <string>
#include <list>
#include <memory>

#include "frame/MUWorkItem.h"
#include "frame/ReturnStatus.h"
#include "frame/MURegister.h"
#include "frame/MUMacros.h"
#include "protocol/protocol.h"
#include "state/RuleManager.h"
#include "state/BucketManager.h"
#include "state/Bucket.h"
#include "data/FileMeta.h"
#include "oplog/LogDispatcher.h"
#include "oplog/LogEvent.h"
#include "item/FileOpItem.h"
#include "item/UserItem.h"
#include "item/LogItem.h"

#include "log/log.h"

ClientTask::~ClientTask()
{

}

ClientTask::ClientTask(MUTask *pParent) :
    PassiveTask(pParent)
{
    m_CurrentState = TASK_WORKING;
}

void
ClientTask::destroy(MUTCPAgent *pChannel)
{
    m_pOwner = NULL;

    recycle();
    m_CurrentState = TASK_RECYCLED;
}

int
ClientTask::next(MUTCPAgent *pAgent, const InReq &req)
{
#ifdef DEBUG
    extern uint64_t g_receivedPerSec;
    ++g_receivedPerSec;
#endif
    int rt = 0;

    rt = dispatch(req);

    return 0;
}

int
ClientTask::next(MUWorkItem *pItem)
{
#ifdef DEBUG
    extern uint64_t g_processedPerSec;
    ++g_processedPerSec;
#endif

    // sub ref cnt
    release();

    int rt = 0;

    rt = dispatch(pItem);

    return 0;
}

int
ClientTask::dispatch(const InReq &req)
{
    switch (req.m_msgHeader.cmd) {

    case MSG_SYS_MU_GET_USER_INFO: {
            return getUserInfo(req);
            break;
        }

    case MSG_SYS_MU_PUT_DIR: {
            return putDir(req);
            break;
        }

    case MSG_SYS_MU_DEL_DIR: {
            return delDir(req);
            break;
        }

    case MSG_SYS_MU_GET_DIR: {
            return getDir(req);
            break;
        }

    case MSG_SYS_MU_STAT_DIR: {
            return statDir(req);
            break;
        }

    case MSG_SYS_MU_GET_DIR2: {
            return getDir2(req);
            break;
        }

    case MSG_SYS_MU_MOV_DIR: {
            return movDir(req);
            break;
        }

    case MSG_SYS_MU_PUT_FILE: {
            return putFile(req);
            break;
        }

    case MSG_SYS_MU_DEL_FILE: {
            return delFile(req);
            break;
        }

    case MSG_SYS_MU_GET_FILE: {
            return getFile(req);
            break;
        }

    case MSG_SYS_MU_MOV_FILE: {
            return movFile(req);
            break;
        }

    case MSG_SYS_MU_GET_USER_LOG: {
            return getUserLog(req);
            break;
        }

    case MSG_MPC_MU_CREATE_USER: {
            return createUser(req);
            break;
        }

    case MSG_MPC_MU_DELETE_USER: {
            return deleteUser(req);
            break;
        }

    default: {
            ERROR_LOG("Unknown protocol command 0x%x.", req.m_msgHeader.cmd);
            return -1;
            break;
        }

    }

    return 0;
}

int
ClientTask::getUserInfo(const InReq &req)
{
    std::string data(req.ioBuf, req.m_msgHeader.length);
    cstore::pb_MSG_SYS_MU_GET_USER_INFO getUserInfo;

    if (!getUserInfo.ParseFromString(data)) {
        ERROR_LOG("Protobuf parse failed, command 0x%x.", req.m_msgHeader.cmd);
        errorResponse(MSG_SYS_MU_GET_USER_INFO_ACK, req.m_msgHeader.para1);
        return -1;
    }

    ReturnStatus rs;
    rs = checkUser(getUserInfo.uid());

    if (!rs.success()) {
        simpleResponse(MSG_SYS_MU_GET_USER_INFO_ACK,
                       rs.errorCode, req.m_msgHeader.para1);
        return 0;
    }

    UserItem *pItem = new UserItem(this);

    pItem->setItemType(USER_ITEM);
    pItem->setWorkType(UserItem::GET_USER_INFO);
    pItem->setRequestId(req.m_msgHeader.para1);
    pItem->setUserId(getUserInfo.uid());
    pItem->setBucketId(
        RuleManager::getInstance()->getBucketIdByUserId(getUserInfo.uid()));

    sendItem(pItem);

    return 0;
}

int
ClientTask::createUser(const InReq &req)
{
    std::string data(req.ioBuf, req.m_msgHeader.length);
    cstore::pb_MSG_MPC_MU_CREATE_USER createUser;

    if (!createUser.ParseFromString(data)) {
        ERROR_LOG("Protobuf parse failed, command 0x%x.", req.m_msgHeader.cmd);
        errorResponse(MSG_MPC_MU_CREATE_USER_ACK, req.m_msgHeader.para1);
        return -1;
    }

    ReturnStatus rs;
    rs = checkUser(createUser.uid());

    if (!rs.success()) {
        simpleResponse(MSG_MPC_MU_CREATE_USER_ACK,
                       rs.errorCode, req.m_msgHeader.para1);
        return 0;
    }

    UserItem *pItem = new UserItem(this);

    pItem->setItemType(USER_ITEM);
    pItem->setWorkType(UserItem::CREATE_USER);
    pItem->setRequestId(req.m_msgHeader.para1);
    pItem->setUserId(createUser.uid());
    pItem->setBucketId(
        RuleManager::getInstance()->getBucketIdByUserId(createUser.uid()));
    pItem->setUserQuota(createUser.quota());

    sendItem(pItem);

    return 0;
}

int
ClientTask::deleteUser(const InReq &req)
{
    std::string data(req.ioBuf, req.m_msgHeader.length);
    cstore::pb_MSG_MPC_MU_DELETE_USER delUser;

    if (!delUser.ParseFromString(data)) {
        ERROR_LOG("Protobuf parse failed, command 0x%x.", req.m_msgHeader.cmd);
        errorResponse(MSG_MPC_MU_DELETE_USER_ACK, req.m_msgHeader.para1);
        return -1;
    }

    ReturnStatus rs;
    rs = checkUser(delUser.uid());

    if (!rs.success()) {
        simpleResponse(MSG_MPC_MU_DELETE_USER_ACK,
                       rs.errorCode, req.m_msgHeader.para1);
        return 0;
    }

    UserItem *pItem = new UserItem(this);

    pItem->setItemType(USER_ITEM);
    pItem->setWorkType(UserItem::DELETE_USER);
    pItem->setRequestId(req.m_msgHeader.para1);
    pItem->setUserId(delUser.uid());
    pItem->setBucketId(
        RuleManager::getInstance()->getBucketIdByUserId(delUser.uid()));

    sendItem(pItem);

    return 0;
}

int
ClientTask::putDir(const InReq &req)
{
    std::string data(req.ioBuf, req.m_msgHeader.length);
    cstore::pb_MSG_SYS_MU_PUT_DIR putDir;

    if (!putDir.ParseFromString(data)) {
        ERROR_LOG("Protobuf parse failed, command 0x%x.", req.m_msgHeader.cmd);
        errorResponse(MSG_SYS_MU_PUT_DIR_ACK, req.m_msgHeader.para1);
        return -1;
    }

    ReturnStatus rs;
    rs = checkUser(putDir.uid());

    if (!rs.success()) {
        simpleResponse(MSG_SYS_MU_PUT_DIR_ACK,
                       rs.errorCode, req.m_msgHeader.para1);
        return 0;
    }

    std::string path = putDir.path();

    if (!checkPath(path) || path == ROOT_PATH) {
        ERROR_LOG("invalid path, %s", path.c_str());
        simpleResponse(MSG_SYS_MU_PUT_DIR_ACK,
                       PATH_INVALID, req.m_msgHeader.para1);
        return 0;
    }

    FileOpItem *pItem = new FileOpItem(this);

    pItem->setItemType(FILE_OP_ITEM);
    pItem->setWorkType(FileOpItem::PUT_DIR);
    pItem->setRequestId(req.m_msgHeader.para1);
    pItem->setUserId(putDir.uid());
    pItem->setBucketId(
        RuleManager::getInstance()->getBucketIdByUserId(putDir.uid()));
    pItem->setToken(putDir.token());
    pItem->setPath(putDir.path());

    sendItem(pItem);

    return 0;
}

int
ClientTask::delDir(const InReq &req)
{
    std::string data(req.ioBuf, req.m_msgHeader.length);
    cstore::pb_MSG_SYS_MU_DEL_DIR delDir;

    if (!delDir.ParseFromString(data)) {
        ERROR_LOG("Protobuf parse failed, command 0x%x.", req.m_msgHeader.cmd);
        errorResponse(MSG_SYS_MU_DEL_DIR_ACK, req.m_msgHeader.para1);
        return -1;
    }

    ReturnStatus rs;
    rs = checkUser(delDir.uid());

    if (!rs.success()) {
        simpleResponse(MSG_SYS_MU_DEL_DIR_ACK,
                       rs.errorCode, req.m_msgHeader.para1);
        return 0;
    }

    std::string path = delDir.path();

    if (!checkPath(path)) {
        ERROR_LOG("invalid path, %s", path.c_str());
        simpleResponse(MSG_SYS_MU_DEL_DIR_ACK,
                       PATH_INVALID, req.m_msgHeader.para1);
        return 0;
    }

    FileOpItem *pItem = new FileOpItem(this);

    pItem->setItemType(FILE_OP_ITEM);
    pItem->setWorkType(FileOpItem::DEL_DIR);
    pItem->setRequestId(req.m_msgHeader.para1);
    pItem->setUserId(delDir.uid());
    pItem->setBucketId(
        RuleManager::getInstance()->getBucketIdByUserId(delDir.uid()));
    pItem->setToken(delDir.token());
    pItem->setPath(delDir.path());

    sendItem(pItem);

    return 0;
}

int
ClientTask::getDir(const InReq &req)
{
    std::string data(req.ioBuf, req.m_msgHeader.length);
    cstore::pb_MSG_SYS_MU_GET_DIR getDir;

    if (!getDir.ParseFromString(data)) {
        ERROR_LOG("Protobuf parse failed, command 0x%x.", req.m_msgHeader.cmd);
        errorResponse(MSG_SYS_MU_GET_DIR_ACK, req.m_msgHeader.para1);
        return -1;
    }

    ReturnStatus rs;
    rs = checkUser(getDir.uid());

    if (!rs.success()) {
        simpleResponse(MSG_SYS_MU_GET_DIR_ACK,
                       rs.errorCode, req.m_msgHeader.para1);
        return 0;
    }

    std::string path = getDir.path();

    if (!checkPath(path)) {
        ERROR_LOG("invalid path, %s", path.c_str());
        simpleResponse(MSG_SYS_MU_GET_DIR_ACK,
                       PATH_INVALID, req.m_msgHeader.para1);
        return 0;
    }

    FileOpItem *pItem = new FileOpItem(this);

    pItem->setItemType(FILE_OP_ITEM);
    pItem->setWorkType(FileOpItem::GET_DIR);
    pItem->setRequestId(req.m_msgHeader.para1);
    pItem->setUserId(getDir.uid());
    pItem->setBucketId(
        RuleManager::getInstance()->getBucketIdByUserId(getDir.uid()));
    pItem->setToken(getDir.token());
    pItem->setPath(getDir.path());

    sendItem(pItem);

    return 0;
}

int
ClientTask::statDir(const InReq &req)
{
    std::string data(req.ioBuf, req.m_msgHeader.length);
    cstore::pb_MSG_SYS_MU_STAT_DIR statDir;

    if (!statDir.ParseFromString(data)) {
        ERROR_LOG("Protobuf parse failed, command 0x%x.", req.m_msgHeader.cmd);
        errorResponse(MSG_SYS_MU_STAT_DIR_ACK, req.m_msgHeader.para1);
        return -1;
    }

    ReturnStatus rs;
    rs = checkUser(statDir.uid());

    if (!rs.success()) {
        simpleResponse(MSG_SYS_MU_STAT_DIR_ACK,
                       rs.errorCode, req.m_msgHeader.para1);
        return 0;
    }

    std::string path = statDir.path();

    if (!checkPath(path)) {
        ERROR_LOG("invalid path, %s", path.c_str());
        simpleResponse(MSG_SYS_MU_STAT_DIR_ACK,
                       PATH_INVALID, req.m_msgHeader.para1);
        return 0;
    }

    FileOpItem *pItem = new FileOpItem(this);

    pItem->setItemType(FILE_OP_ITEM);
    pItem->setWorkType(FileOpItem::STAT_DIR);
    pItem->setRequestId(req.m_msgHeader.para1);
    pItem->setUserId(statDir.uid());
    pItem->setBucketId(
        RuleManager::getInstance()->getBucketIdByUserId(statDir.uid()));
    pItem->setToken(statDir.token());
    pItem->setPath(statDir.path());

    sendItem(pItem);

    return 0;
}

int
ClientTask::getDir2(const InReq &req)
{
    std::string data(req.ioBuf, req.m_msgHeader.length);
    cstore::pb_MSG_SYS_MU_GET_DIR2 getDir2;

    if (!getDir2.ParseFromString(data)) {
        ERROR_LOG("Protobuf parse failed, command 0x%x.", req.m_msgHeader.cmd);
        errorResponse(MSG_SYS_MU_GET_DIR2_ACK, req.m_msgHeader.para1);
        return -1;
    }

    ReturnStatus rs;
    rs = checkUser(getDir2.uid());

    if (!rs.success()) {
        simpleResponse(MSG_SYS_MU_GET_DIR2_ACK,
                       rs.errorCode, req.m_msgHeader.para1);
        return 0;
    }

    std::string path = getDir2.path();

    if (!checkPath(path)) {
        ERROR_LOG("invalid path, %s", path.c_str());
        simpleResponse(MSG_SYS_MU_GET_DIR2_ACK,
                       PATH_INVALID, req.m_msgHeader.para1);
        return 0;
    }

    FileOpItem *pItem = new FileOpItem(this);

    pItem->setItemType(FILE_OP_ITEM);
    pItem->setWorkType(FileOpItem::GET_DIR2);
    pItem->setRequestId(req.m_msgHeader.para1);
    pItem->setUserId(getDir2.uid());
    pItem->setBucketId(
        RuleManager::getInstance()->getBucketIdByUserId(getDir2.uid()));
    pItem->setToken(getDir2.token());
    pItem->setPath(getDir2.path());

    sendItem(pItem);

    return 0;
}

int
ClientTask::movDir(const InReq &req)
{
    std::string data(req.ioBuf, req.m_msgHeader.length);
    cstore::pb_MSG_SYS_MU_MOV_DIR movDir;

    if (!movDir.ParseFromString(data)) {
        ERROR_LOG("Protobuf parse failed, command 0x%x.", req.m_msgHeader.cmd);
        errorResponse(MSG_SYS_MU_MOV_DIR_ACK, req.m_msgHeader.para1);
        return -1;
    }

    ReturnStatus rs;
    rs = checkUser(movDir.uid());

    if (!rs.success()) {
        simpleResponse(MSG_SYS_MU_MOV_DIR_ACK,
                       rs.errorCode, req.m_msgHeader.para1);
        return 0;
    }

    std::string srcPath = movDir.src_path();
    std::string destPath = movDir.dest_path();

    if (!checkPath(srcPath) || srcPath == ROOT_PATH) {
        ERROR_LOG("invalid path, %s", srcPath.c_str());
        simpleResponse(MSG_SYS_MU_MOV_DIR_ACK,
                       SRC_PATH_INVALID, req.m_msgHeader.para1);
        return 0;
    }

    if (!checkPath(destPath) || destPath == ROOT_PATH) {
        ERROR_LOG("invalid path, %s", destPath.c_str());
        simpleResponse(MSG_SYS_MU_MOV_DIR_ACK,
                       DEST_PATH_INVALID, req.m_msgHeader.para1);
        return 0;
    }

    FileOpItem *pItem = new FileOpItem(this);

    pItem->setItemType(FILE_OP_ITEM);
    pItem->setWorkType(FileOpItem::MOV_DIR);
    pItem->setRequestId(req.m_msgHeader.para1);
    pItem->setUserId(movDir.uid());
    pItem->setBucketId(
        RuleManager::getInstance()->getBucketIdByUserId(movDir.uid()));
    pItem->setToken(movDir.token());
    pItem->setSrcPath(movDir.src_path());
    pItem->setDestPath(movDir.dest_path());

    sendItem(pItem);

    return 0;
}

int
ClientTask::putFile(const InReq &req)
{
    std::string data(req.ioBuf, req.m_msgHeader.length);
    cstore::pb_MSG_SYS_MU_PUT_FILE putFile;

    if (!putFile.ParseFromString(data)) {
        ERROR_LOG("Protobuf parse failed, command 0x%x.", req.m_msgHeader.cmd);
        errorResponse(MSG_SYS_MU_PUT_FILE_ACK, req.m_msgHeader.para1);
        return -1;
    }

    ReturnStatus rs;
    rs = checkUser(putFile.uid());

    if (!rs.success()) {
        simpleResponse(MSG_SYS_MU_PUT_FILE_ACK,
                       rs.errorCode, req.m_msgHeader.para1);
        return 0;
    }

    std::string path = putFile.path();

    if (!checkPath(path) || path == ROOT_PATH) {
        ERROR_LOG("invalid path, %s", path.c_str());
        simpleResponse(MSG_SYS_MU_PUT_FILE_ACK,
                       PATH_INVALID, req.m_msgHeader.para1);
        return 0;
    }

    FileOpItem *pItem = new FileOpItem(this);

    pItem->setItemType(FILE_OP_ITEM);
    pItem->setWorkType(FileOpItem::PUT_FILE);
    pItem->setRequestId(req.m_msgHeader.para1);
    pItem->setUserId(putFile.uid());
    pItem->setBucketId(
        RuleManager::getInstance()->getBucketIdByUserId(putFile.uid()));
    pItem->setToken(putFile.token());
    pItem->setPath(putFile.path());
    //cout <<"ClientTask::putFile(const InReq &req) path="<<putFile.path()<<endl;

    const cstore::File_Attr &attr = putFile.attr();

    FileMeta meta;
    meta.m_Attr.m_Version = attr.version();
    meta.m_Attr.m_Mode = attr.mode();
    meta.m_Attr.m_CTime = attr.ctime();
    meta.m_Attr.m_MTime = attr.mtime();
    meta.m_Attr.m_Type = attr.type();
    meta.m_Attr.m_Size = attr.size();

    int blockListSize = putFile.block_list_size();

    for (int i = 0; i < blockListSize; ++i) {
        BlockMeta bmeta;
        bmeta.m_Checksum = putFile.block_list(i).checksum();
        meta.m_BlockList.push_back(bmeta);
    }

    pItem->setFileMetaIn(meta);

    sendItem(pItem);

    return 0;
}

int
ClientTask::delFile(const InReq &req)
{
    std::string data(req.ioBuf, req.m_msgHeader.length);
    cstore::pb_MSG_SYS_MU_DEL_FILE delFile;

    if (!delFile.ParseFromString(data)) {
        ERROR_LOG("Protobuf parse failed, command 0x%x.", req.m_msgHeader.cmd);
        errorResponse(MSG_SYS_MU_DEL_FILE_ACK, req.m_msgHeader.para1);
        return -1;
    }

    ReturnStatus rs;
    rs = checkUser(delFile.uid());

    if (!rs.success()) {
        simpleResponse(MSG_SYS_MU_DEL_FILE_ACK,
                       rs.errorCode, req.m_msgHeader.para1);
        return 0;
    }

    std::string path = delFile.path();

    if (!checkPath(path) || path == ROOT_PATH) {
        ERROR_LOG("invalid path, %s", path.c_str());
        simpleResponse(MSG_SYS_MU_DEL_FILE_ACK,
                       PATH_INVALID, req.m_msgHeader.para1);
        return 0;
    }

    FileOpItem *pItem = new FileOpItem(this);

    pItem->setItemType(FILE_OP_ITEM);
    pItem->setWorkType(FileOpItem::DEL_FILE);
    pItem->setRequestId(req.m_msgHeader.para1);
    pItem->setUserId(delFile.uid());
    pItem->setBucketId(
        RuleManager::getInstance()->getBucketIdByUserId(delFile.uid()));
    pItem->setToken(delFile.token());
    pItem->setPath(delFile.path());

    sendItem(pItem);

    return 0;
}

int
ClientTask::getFile(const InReq &req)
{
    std::string data(req.ioBuf, req.m_msgHeader.length);
    cstore::pb_MSG_SYS_MU_GET_FILE getFile;

    if (!getFile.ParseFromString(data)) {
        ERROR_LOG("Protobuf parse failed, command 0x%x.", req.m_msgHeader.cmd);
        errorResponse(MSG_SYS_MU_GET_FILE_ACK, req.m_msgHeader.para1);
        return -1;
    }

    ReturnStatus rs;
    rs = checkUser(getFile.uid());

    if (!rs.success()) {
        simpleResponse(MSG_SYS_MU_GET_FILE_ACK,
                       rs.errorCode, req.m_msgHeader.para1);
        return 0;
    }

    std::string path = getFile.path();

    if (!checkPath(path) || path == ROOT_PATH) {
        ERROR_LOG("invalid path, %s", path.c_str());
        simpleResponse(MSG_SYS_MU_GET_FILE_ACK,
                       PATH_INVALID, req.m_msgHeader.para1);
        return 0;
    }

    FileOpItem *pItem = new FileOpItem(this);

    pItem->setItemType(FILE_OP_ITEM);
    pItem->setWorkType(FileOpItem::GET_FILE);
    pItem->setRequestId(req.m_msgHeader.para1);
    pItem->setUserId(getFile.uid());
    pItem->setBucketId(
        RuleManager::getInstance()->getBucketIdByUserId(getFile.uid()));
    pItem->setToken(getFile.token());
    pItem->setPath(getFile.path());

    sendItem(pItem);

    return 0;
}

int
ClientTask::movFile(const InReq &req)
{
    std::string data(req.ioBuf, req.m_msgHeader.length);
    cstore::pb_MSG_SYS_MU_MOV_FILE movFile;

    if (!movFile.ParseFromString(data)) {
        ERROR_LOG("Protobuf parse failed, command 0x%x.", req.m_msgHeader.cmd);
        errorResponse(MSG_SYS_MU_MOV_FILE_ACK, req.m_msgHeader.para1);
        return -1;
    }

    ReturnStatus rs;
    rs = checkUser(movFile.uid());

    if (!rs.success()) {
        simpleResponse(MSG_SYS_MU_MOV_FILE_ACK,
                       rs.errorCode, req.m_msgHeader.para1);
        return 0;
    }

    std::string srcPath = movFile.src_path();
    std::string destPath = movFile.dest_path();

    if (!checkPath(srcPath) || srcPath == ROOT_PATH) {
        ERROR_LOG("invalid path, %s", srcPath.c_str());
        simpleResponse(MSG_SYS_MU_MOV_FILE_ACK,
                       SRC_PATH_INVALID, req.m_msgHeader.para1);
        return 0;
    }

    if (!checkPath(destPath) || destPath == ROOT_PATH) {
        ERROR_LOG("invalid path, %s", destPath.c_str());
        simpleResponse(MSG_SYS_MU_MOV_FILE_ACK,
                       DEST_PATH_INVALID, req.m_msgHeader.para1);
        return 0;
    }

    FileOpItem *pItem = new FileOpItem(this);

    pItem->setItemType(FILE_OP_ITEM);
    pItem->setWorkType(FileOpItem::MOV_FILE);
    pItem->setRequestId(req.m_msgHeader.para1);
    pItem->setUserId(movFile.uid());
    pItem->setBucketId(
        RuleManager::getInstance()->getBucketIdByUserId(movFile.uid()));
    pItem->setToken(movFile.token());
    pItem->setSrcPath(movFile.src_path());
    pItem->setDestPath(movFile.dest_path());

    sendItem(pItem);

    return 0;
}

int
ClientTask::getUserLog(const InReq &req)
{
    std::string data(req.ioBuf, req.m_msgHeader.length);
    cstore::pb_MSG_SYS_MU_GET_USER_LOG getUserLog;

    if (!getUserLog.ParseFromString(data)) {
        ERROR_LOG("Protobuf parse failed, command 0x%x.", req.m_msgHeader.cmd);
        errorResponse(MSG_SYS_MU_GET_USER_LOG_ACK, req.m_msgHeader.para1);
        return -1;
    }

    ReturnStatus rs;
    rs = checkUser(getUserLog.uid());

    if (!rs.success()) {
        simpleResponse(MSG_SYS_MU_GET_USER_LOG_ACK,
                       rs.errorCode, req.m_msgHeader.para1);
        return 0;
    }

    LogItem *pItem = new LogItem(this);

    pItem->setItemType(LOG_ITEM);
    pItem->setWorkType(LogItem::GET_USER_LOG);
    pItem->setRequestId(req.m_msgHeader.para1);
    pItem->setUserId(getUserLog.uid());
    pItem->setBucketId(
        RuleManager::getInstance()->getBucketIdByUserId(getUserLog.uid()));
    pItem->setUserLogSeq(getUserLog.seq_nr());

    sendItem(pItem);

    return 0;

}

int
ClientTask::dispatch(MUWorkItem *pItem)
{
    if (FILE_OP_ITEM == pItem->getItemType()) {

        return dispatchFileOpItem(pItem);

    } else if (USER_ITEM == pItem->getItemType()) {

        return dispatchUserItem(pItem);

    } else if (LOG_ITEM == pItem->getItemType()) {

        return dispatchLogItem(pItem);

    } else {
        ERROR_LOG("Unknown work item type %d.", pItem->getItemType());
        return -1;
    }

    return 0;
}

int
ClientTask::dispatchFileOpItem(MUWorkItem *pItem)
{
    if (TASK_WORKING != m_CurrentState) {
        switch (pItem->getWorkType()) {

        case FileOpItem::PUT_DIR:
        case FileOpItem::DEL_DIR:
        case FileOpItem::MOV_DIR:
        case FileOpItem::PUT_FILE:
        case FileOpItem::DEL_FILE:
        case FileOpItem::MOV_FILE: {
                // have to log operations even after task recycled
                break;
            }

        default: {
                ERROR_LOG("unexpected task state %d", m_CurrentState);
                delete pItem;
                pItem = NULL;
                return -1;
            }

        }

    }

    switch (pItem->getWorkType()) {

    case FileOpItem::PUT_DIR: {
            return putDir(pItem);
            break;
        }

    case FileOpItem::DEL_DIR: {
            return delDir(pItem);
            break;
        }

    case FileOpItem::GET_DIR: {
            return getDir(pItem);
            break;
        }

    case FileOpItem::STAT_DIR: {
            return statDir(pItem);
            break;
        }

    case FileOpItem::GET_DIR2: {
            return getDir2(pItem);
            break;
        }

    case FileOpItem::MOV_DIR: {
            return movDir(pItem);
            break;
        }

    case FileOpItem::PUT_FILE: {
            return putFile(pItem);
            break;
        }

    case FileOpItem::DEL_FILE: {
            return delFile(pItem);
            break;
        }

    case FileOpItem::MOV_FILE: {
            return movFile(pItem);
            break;
        }

    case FileOpItem::GET_FILE: {
            return getFile(pItem);
            break;
        }

    default: {
            ERROR_LOG("Unknown FileOpItem work type %d.",
                      pItem->getWorkType());
            return -1;
            break;
        }

    }

    return 0;
}

int
ClientTask::dispatchUserItem(MUWorkItem *pItem)
{
    if (TASK_WORKING != m_CurrentState) {
        switch (pItem->getWorkType()) {

        case UserItem::CREATE_USER:
        case UserItem::DELETE_USER: {
                // have to log operations even after task recycled
                break;
            }

        default: {
                ERROR_LOG("unexpected task state %d", m_CurrentState);
                delete pItem;
                pItem = NULL;
                return -1;
            }

        }

    }

    switch (pItem->getWorkType()) {

    case UserItem::CREATE_USER: {
            return createUser(pItem);
            break;
        }

    case UserItem::DELETE_USER: {
            return deleteUser(pItem);
            break;
        }

    case UserItem::GET_USER_INFO: {
            return getUserInfo(pItem);
            break;
        }

    default: {
            ERROR_LOG("Unknown UserItem work type %d.",
                      pItem->getWorkType());
            return -1;
            break;
        }

    }

    return 0;
}

int
ClientTask::dispatchLogItem(MUWorkItem *pItem)
{
    if (TASK_WORKING != m_CurrentState) {
        ERROR_LOG("unexpected task state %d", m_CurrentState);
        delete pItem;
        pItem = NULL;
        return -1;
    }

    switch (pItem->getWorkType()) {

    case LogItem::GET_USER_LOG: {
            return getUserLog(pItem);
            break;
        }

    default: {
            ERROR_LOG("Unknown LogItem work type %d.",
                      pItem->getWorkType());
            return -1;
            break;
        }

    }

    return 0;
}

int
ClientTask::getUserInfo(MUWorkItem *pItem)
{
    std::auto_ptr<UserItem> pRItem =
        std::auto_ptr<UserItem>(dynamic_cast<UserItem *>(pItem));

    ReturnStatus rs = pRItem->getReturnStatus();

    MsgHeader msg;
    msg.cmd = MSG_SYS_MU_GET_USER_INFO_ACK;
    msg.para1 = pRItem->getRequestId();

    if (rs.success()) {
        msg.error = MU_OK;

        cstore::pb_MSG_SYS_MU_GET_USER_INFO_ACK getUserInfoAck;

        getUserInfoAck.set_seq_nr(pRItem->getUserLogSeq());

        const UserInfo &info = pRItem->getUserInfo();
        getUserInfoAck.set_quota_total(info.m_TotalQuota);
        getUserInfoAck.set_quota_used(info.m_UsedQuota);

        std::string data;

        if (!getUserInfoAck.SerializeToString(&data)) {
            ERROR_LOG("Protobuf serialize failed, command 0x%x.", msg.cmd);
            errorResponse(msg.cmd, msg.para1);
            return -1;
        }

        msg.length = data.length();

        m_pOwner->sendPacket(msg, data.c_str());

    } else {
        ERROR_LOG("get user info failed, user id %llu, bucket id %llu",
                  pRItem->getUserId(), pRItem->getBucketId());
        msg.error = rs.errorCode;
    }

    msg.length = 0;

    m_pOwner->sendPacket(msg, NULL);

    return 0;
}

int
ClientTask::putDir(MUWorkItem *pItem)
{
    std::auto_ptr<FileOpItem> pRItem =
        std::auto_ptr<FileOpItem>(dynamic_cast<FileOpItem *>(pItem));

    ReturnStatus rs = pRItem->getReturnStatus();

    if (TASK_WORKING != m_CurrentState) {
        if (rs.success()) {
            ERROR_LOG("log put dir operation after task recycled");
            logPutDir(pItem);
        }

        return 0;
    }

    MsgHeader msg;
    msg.cmd = MSG_SYS_MU_PUT_DIR_ACK;

    if (rs.success()) {
        msg.error = MU_OK;

        logPutDir(pItem);

    } else {
        ERROR_LOG("put dir failed, user id %llu, bucket id %llu, path %s",
                  pRItem->getUserId(), pRItem->getBucketId(),
                  pRItem->getPath().c_str());
        msg.error = rs.errorCode;
    }

    msg.length = 0;
    msg.para1 = pRItem->getRequestId();

    m_pOwner->sendPacket(msg, NULL);

    return 0;
}

int
ClientTask::delDir(MUWorkItem *pItem)
{
    std::auto_ptr<FileOpItem> pRItem =
        std::auto_ptr<FileOpItem>(dynamic_cast<FileOpItem *>(pItem));

    ReturnStatus rs = pRItem->getReturnStatus();

    if (TASK_WORKING != m_CurrentState) {
        if (rs.success()) {
            ERROR_LOG("log del dir operation after task recycled");
            logDelDir(pItem);
        }

        return 0;
    }

    MsgHeader msg;
    msg.cmd = MSG_SYS_MU_DEL_DIR_ACK;

    if (rs.success()) {
        msg.error = MU_OK;

        logDelDir(pItem);

    } else {
        ERROR_LOG("del dir failed, user id %llu, bucket id %llu, path %s",
                  pRItem->getUserId(), pRItem->getBucketId(),
                  pRItem->getPath().c_str());
        msg.error = rs.errorCode;
    }

    msg.length = 0;
    msg.para1 = pRItem->getRequestId();

    m_pOwner->sendPacket(msg, NULL);

    return 0;
}

int
ClientTask::getDir(MUWorkItem *pItem)
{
    std::auto_ptr<FileOpItem> pRItem =
        std::auto_ptr<FileOpItem>(dynamic_cast<FileOpItem *>(pItem));


    ReturnStatus rs = pRItem->getReturnStatus();

    MsgHeader msg;
    msg.cmd = MSG_SYS_MU_GET_DIR_ACK;
    msg.para1 = pRItem->getRequestId();

    if (rs.success()) {
        msg.error = MU_OK;

        cstore::pb_MSG_SYS_MU_GET_DIR_ACK getDirAck;
        cstore::pb_MSG_SYS_MU_GET_DIR_ACK_DEntry *pEntry = NULL;

        const std::list<PDEntry> &dentryList = pRItem->getPDEntryList();

        for (std::list<PDEntry>::const_iterator it = dentryList.begin();
             it != dentryList.end(); ++it) {
            pEntry = getDirAck.add_dentry_list();
            pEntry->set_name(it->m_Name);
            pEntry->set_type(it->m_Type);
        }

        std::string data;

        if (!getDirAck.SerializeToString(&data)) {
            ERROR_LOG("Protobuf serialize failed, command 0x%x.", msg.cmd);
            errorResponse(msg.cmd, msg.para1);
            return -1;
        }

        msg.length = data.length();

        m_pOwner->sendPacket(msg, data.c_str());

    } else {
        ERROR_LOG("get dir failed, user id %llu, bucket id %llu, path %s",
                  pRItem->getUserId(), pRItem->getBucketId(),
                  pRItem->getPath().c_str());
        msg.error = rs.errorCode;
        msg.length = 0;

        m_pOwner->sendPacket(msg, NULL);
    }

    return 0;
}

int
ClientTask::statDir(MUWorkItem *pItem)
{
    std::auto_ptr<FileOpItem> pRItem =
        std::auto_ptr<FileOpItem>(dynamic_cast<FileOpItem *>(pItem));

    ReturnStatus rs = pRItem->getReturnStatus();

    MsgHeader msg;
    msg.cmd = MSG_SYS_MU_STAT_DIR_ACK;
    msg.para1 = pRItem->getRequestId();

    if (rs.success()) {
        msg.error = MU_OK;

        cstore::pb_MSG_SYS_MU_STAT_DIR_ACK statDirAck;

        const FileMeta &meta = pRItem->getFileMetaOut();
        statDirAck.set_mode(meta.m_Attr.m_Mode);
        statDirAck.set_ctime(meta.m_Attr.m_CTime);
        statDirAck.set_mtime(meta.m_Attr.m_MTime);
        statDirAck.set_size(meta.m_Attr.m_Size);

        std::string data;

        if (!statDirAck.SerializeToString(&data)) {
            ERROR_LOG("Protobuf serialize failed, command 0x%x.", msg.cmd);
            errorResponse(msg.cmd, msg.para1);
            return -1;
        }

        msg.length = data.length();

        m_pOwner->sendPacket(msg, data.c_str());

    } else {
        ERROR_LOG("stat dir failed, user id %llu, bucket id %llu, path %s",
                  pRItem->getUserId(), pRItem->getBucketId(),
                  pRItem->getPath().c_str());
        msg.error = rs.errorCode;
        msg.length = 0;

        m_pOwner->sendPacket(msg, NULL);
    }

    return 0;
}

int
ClientTask::getDir2(MUWorkItem *pItem)
{
    std::auto_ptr<FileOpItem> pRItem =
        std::auto_ptr<FileOpItem>(dynamic_cast<FileOpItem *>(pItem));

    ReturnStatus rs = pRItem->getReturnStatus();

    MsgHeader msg;
    msg.cmd = MSG_SYS_MU_GET_DIR2_ACK;
    msg.para1 = pRItem->getRequestId();

    if (rs.success()) {
        cstore::pb_MSG_SYS_MU_GET_DIR2_ACK getDir2Ack;
        cstore::pb_MSG_SYS_MU_GET_DIR2_ACK_DEntry *pEntry = NULL;
        cstore::Block_Meta *pBlock = NULL;

        const std::list<EDEntry> &entryList = pRItem->getEDEntryList();

        for (std::list<EDEntry>::const_iterator it = entryList.begin();
             it != entryList.end(); ++it) {
            pEntry = getDir2Ack.add_dentry_list();
            pEntry->set_type(it->m_Type);
            pEntry->set_name(it->m_Name);
            pEntry->set_mode(it->m_Mode);
            pEntry->set_ctime(it->m_CTime);
            pEntry->set_mtime(it->m_MTime);
            pEntry->set_size(it->m_Size);
            pEntry->set_version(it->m_Version);

            for (std::list<BlockMeta>::const_iterator bit =
                     it->m_BlockList.begin(); bit != it->m_BlockList.end();
                 ++bit) {
                pBlock = pEntry->add_block_list();
                pBlock->set_checksum(bit->m_Checksum);
            }
        }

        std::string data;

        if (!getDir2Ack.SerializeToString(&data)) {
            ERROR_LOG("Protobuf serialize failed, command 0x%x.", msg.cmd);
            errorResponse(msg.cmd, msg.para1);
            return -1;
        }

        msg.length = data.length();

        m_pOwner->sendPacket(msg, data.c_str());

    } else {
        ERROR_LOG("get dir2 failed, user id %llu, bucket id %llu, path %s",
                  pRItem->getUserId(), pRItem->getBucketId(),
                  pRItem->getPath().c_str());
        msg.error = rs.errorCode;
        msg.length = 0;

        m_pOwner->sendPacket(msg, NULL);
    }

    return 0;
}

int
ClientTask::movDir(MUWorkItem *pItem)
{
    std::auto_ptr<FileOpItem> pRItem =
        std::auto_ptr<FileOpItem>(dynamic_cast<FileOpItem *>(pItem));

    ReturnStatus rs = pRItem->getReturnStatus();

    if (TASK_WORKING != m_CurrentState) {
        if (rs.success()) {
            ERROR_LOG("log mov dir operation after task recycled");
            logMovDir(pItem);
        }

        return 0;
    }

    MsgHeader msg;
    msg.cmd = MSG_SYS_MU_MOV_DIR_ACK;
    msg.para1 = pRItem->getRequestId();

    if (rs.success()) {
        msg.error = MU_OK;

        logMovDir(pItem);

    } else {
        ERROR_LOG("mov dir failed, user id %llu, bucket id %llu, "
                  "src path %s, dest path %s",
                  pRItem->getUserId(), pRItem->getBucketId(),
                  pRItem->getSrcPath().c_str(), pRItem->getDestPath().c_str());
        msg.error = rs.errorCode;
    }

    msg.length = 0;
    m_pOwner->sendPacket(msg, NULL);

    return 0;
}

int
ClientTask::putFile(MUWorkItem *pItem)
{
    std::auto_ptr<FileOpItem> pRItem =
        std::auto_ptr<FileOpItem>(dynamic_cast<FileOpItem *>(pItem));

    ReturnStatus rs = pRItem->getReturnStatus();

    if (TASK_WORKING != m_CurrentState) {
        if (rs.success()) {
            ERROR_LOG("log put file operation after task recycled");
            logPutFile(pItem);
        }

        return 0;
    }

    MsgHeader msg;
    msg.cmd = MSG_SYS_MU_PUT_FILE_ACK;
    msg.para1 = pRItem->getRequestId();

    if (rs.success()) {
        msg.error = MU_OK;
        msg.length = 0;

        m_pOwner->sendPacket(msg, NULL);

        logPutFile(pItem);

    } else {
        msg.error = rs.errorCode;

        if (VERSION_OUTDATED != rs.errorCode
            && PATH_EXIST != rs.errorCode) {
            ERROR_LOG("put file failed, user id %llu, bucket id %llu, path %s",
                      pRItem->getUserId(), pRItem->getBucketId(),
                      pRItem->getPath().c_str());

            msg.length = 0;
            m_pOwner->sendPacket(msg, NULL);

        } else {
            // version outdated or file exists

            ERROR_LOG("put file failed, version outdated, "
                      "user id %llu, bucket id %llu, path %s",
                      pRItem->getUserId(), pRItem->getBucketId(),
                      pRItem->getPath().c_str());

            cstore::pb_MSG_SYS_MU_PUT_FILE_ACK putFileAck;

            const FileMeta &meta = pRItem->getFileMetaOut();
            putFileAck.set_version(meta.m_Attr.m_Version);

            std::string data;

            if (!putFileAck.SerializeToString(&data)) {
                ERROR_LOG("Protobuf serialize failed, command 0x%x.", msg.cmd);
                errorResponse(msg.cmd, msg.para1);
                return -1;
            }

            msg.length = data.length();

            m_pOwner->sendPacket(msg, data.c_str());
        }
    }

    return 0;
}

int
ClientTask::delFile(MUWorkItem *pItem)
{
    std::auto_ptr<FileOpItem> pRItem =
        std::auto_ptr<FileOpItem>(dynamic_cast<FileOpItem *>(pItem));

    ReturnStatus rs = pRItem->getReturnStatus();

    if (TASK_WORKING != m_CurrentState) {
        if (rs.success()) {
            ERROR_LOG("log del file operation after task recycled");
            logDelFile(pItem);
        }

        return 0;
    }

    MsgHeader msg;
    msg.cmd = MSG_SYS_MU_DEL_FILE_ACK;
    msg.para1 = pRItem->getRequestId();

    if (rs.success()) {
        msg.error = MU_OK;

        logDelFile(pItem);

    } else {
        ERROR_LOG("del file failed, user id %llu, bucket id %llu, path %s",
                  pRItem->getUserId(), pRItem->getBucketId(),
                  pRItem->getPath().c_str());
        msg.error = rs.errorCode;
    }

    msg.length = 0;

    m_pOwner->sendPacket(msg, NULL);

    return 0;
}

int
ClientTask::getFile(MUWorkItem *pItem)
{
    std::auto_ptr<FileOpItem> pRItem =
        std::auto_ptr<FileOpItem>(dynamic_cast<FileOpItem *>(pItem));

    ReturnStatus rs = pRItem->getReturnStatus();

    MsgHeader msg;
    msg.cmd = MSG_SYS_MU_GET_FILE_ACK;
    msg.para1 = pRItem->getRequestId();

    if (rs.success()) {
        msg.error = MU_OK;

        cstore::pb_MSG_SYS_MU_GET_FILE_ACK getFileAck;
        cstore::File_Attr *pAttr = NULL;
        cstore::Block_Meta *pBlock = NULL;

        const FileMeta &meta = pRItem->getFileMetaOut();
        pAttr = getFileAck.mutable_attr();

        pAttr->set_version(meta.m_Attr.m_Version);
        pAttr->set_mode(meta.m_Attr.m_Mode);
        pAttr->set_ctime(meta.m_Attr.m_CTime);
        pAttr->set_mtime(meta.m_Attr.m_MTime);
        pAttr->set_type(meta.m_Attr.m_Type);
        pAttr->set_size(meta.m_Attr.m_Size);

        for (std::list<BlockMeta>::const_iterator it =
                 meta.m_BlockList.begin(); it != meta.m_BlockList.end(); ++it) {
            pBlock = getFileAck.add_block_list();
            pBlock->set_checksum(it->m_Checksum);
        }

        std::string data;

        if (!getFileAck.SerializeToString(&data)) {
            ERROR_LOG("Protobuf serialize failed, command 0x%x.", msg.cmd);
            errorResponse(msg.cmd, msg.para1);
            return -1;
        }

        msg.length = data.length();

        m_pOwner->sendPacket(msg, data.c_str());

    } else {
        ERROR_LOG("get file failed, user id %llu, bucket id %llu, path %s",
                  pRItem->getUserId(), pRItem->getBucketId(),
                  pRItem->getPath().c_str());
        msg.error = rs.errorCode;
        msg.length = 0;

        m_pOwner->sendPacket(msg, NULL);
    }

    return 0;
}

int
ClientTask::movFile(MUWorkItem *pItem)
{
    std::auto_ptr<FileOpItem> pRItem =
        std::auto_ptr<FileOpItem>(dynamic_cast<FileOpItem *>(pItem));

    ReturnStatus rs = pRItem->getReturnStatus();

    if (TASK_WORKING != m_CurrentState) {
        if (rs.success()) {
            ERROR_LOG("log mov file operation after task recycled");
            logMovFile(pItem);
        }

        return 0;
    }

    MsgHeader msg;
    msg.cmd = MSG_SYS_MU_MOV_FILE_ACK;
    msg.para1 = pRItem->getRequestId();

    if (rs.success()) {
        msg.error = MU_OK;

        logMovFile(pItem);

    } else {
        ERROR_LOG("mov file failed, user id %llu, bucket id %llu, "
                  "src path %s, dest path %s",
                  pRItem->getUserId(), pRItem->getBucketId(),
                  pRItem->getSrcPath().c_str(), pRItem->getDestPath().c_str());
        msg.error = rs.errorCode;
    }

    msg.length = 0;

    m_pOwner->sendPacket(msg, NULL);

    return 0;
}

int
ClientTask::getUserLog(MUWorkItem *pItem)
{
    std::auto_ptr<LogItem> pRItem =
        std::auto_ptr<LogItem>(dynamic_cast<LogItem *>(pItem));

    ReturnStatus rs = pRItem->getReturnStatus();

    MsgHeader msg;
    msg.cmd = MSG_SYS_MU_GET_USER_LOG_ACK;
    msg.para1 = pRItem->getRequestId();

    if (rs.success()) {
        msg.error = MU_OK;

        // still have more logs to read ?
        if (pRItem->moreLogs()) {
            msg.para2 = MU_MORE_DATA;
            TRACE_LOG("still have more logs to read");

        } else {
            msg.para2 = MU_NO_MORE_DATA;
        }

        cstore::pb_MSG_SYS_MU_GET_USER_LOG_ACK getUserLogAck;
        cstore::Log_Entry *pEntry = NULL;

        const std::list<LogEvent> &logList = pRItem->getLogList();

        TRACE_LOG("get user log, size %" PRIi32, logList.size());

        for (std::list<LogEvent>::const_iterator it = logList.begin();
             it != logList.end(); ++it) {
            pEntry = getUserLogAck.add_log_list();

            if (!pEntry->ParseFromString(it->m_SerializedLog)) {
                ERROR_LOG("protobuf parse error");
                errorResponse(MSG_SYS_MU_GET_USER_LOG_ACK, msg.para1);
                return -1;
            }
        }

        std::string data;

        if (!getUserLogAck.SerializeToString(&data)) {
            ERROR_LOG("protobuf serialize error");
            errorResponse(MSG_SYS_MU_GET_USER_LOG_ACK, msg.para1);
            return -1;
        }

        msg.length = data.length();

        m_pOwner->sendPacket(msg, data.c_str());

    } else {
        msg.error = rs.errorCode;
    }

    msg.length = 0;

    m_pOwner->sendPacket(msg, NULL);

    return 0;
}

int
ClientTask::createUser(MUWorkItem *pItem)
{
    std::auto_ptr<UserItem> pRItem =
        std::auto_ptr<UserItem>(dynamic_cast<UserItem *>(pItem));

    ReturnStatus rs = pRItem->getReturnStatus();

    if (TASK_WORKING != m_CurrentState) {
        if (rs.success()) {
            ERROR_LOG("log create user operation after task recycled");
            logCreateUser(pItem);
        }

        return 0;
    }

    MsgHeader msg;
    msg.cmd = MSG_MPC_MU_CREATE_USER_ACK;
    msg.para1 = pRItem->getRequestId();

    if (rs.success()) {
        msg.error = MU_OK;
        logCreateUser(pItem);

    } else {
        msg.error = rs.errorCode;
    }

    msg.length = 0;

    m_pOwner->sendPacket(msg, NULL);

    return 0;
}

int
ClientTask::deleteUser(MUWorkItem *pItem)
{
    std::auto_ptr<UserItem> pRItem =
        std::auto_ptr<UserItem>(dynamic_cast<UserItem *>(pItem));

    ReturnStatus rs = pRItem->getReturnStatus();

    if (TASK_WORKING != m_CurrentState) {

        if (rs.success()) {
            ERROR_LOG("log delete user operation after task recycled");
            logDeleteUser(pItem);
        }

        return 0;
    }

    MsgHeader msg;
    msg.cmd = MSG_MPC_MU_DELETE_USER_ACK;
    msg.para1 = pRItem->getRequestId();

    if (rs.success()) {
        msg.error = MU_OK;
        logDeleteUser(pItem);

    } else {
        msg.error = rs.errorCode;
    }

    msg.length = 0;

    m_pOwner->sendPacket(msg, NULL);

    return 0;
}

void
ClientTask::errorResponse(uint32_t cmd, uint32_t requestId)
{
    simpleResponse(cmd, MU_UNKNOWN_ERROR, requestId);
}

void
ClientTask::simpleResponse(uint32_t cmd, uint32_t error, uint32_t requestId)
{
    MsgHeader msg;

    msg.cmd = cmd;
    msg.error = error;
    msg.length = 0;
    msg.para1 = requestId;

    m_pOwner->sendPacket(msg, NULL);
}

ReturnStatus
ClientTask::checkUser(uint64_t userId)
{
    uint64_t bucketId =
        RuleManager::getInstance()->getBucketIdByUserId(userId);

    //TRACE_LOG("check user, user id %" PRIu64 ", bucket id %" PRIu64,
    //userId, bucketId);

    Bucket *pBucket =
        BucketManager::getInstance()->get(bucketId);

    if (NULL == pBucket) {
        ERROR_LOG("user id %" PRIu64 ", locate error, "
                  "no such bucket on current MU",
                  userId);
        return ReturnStatus(MU_FAILED, MU_LOCATE_ERROR);
    }

    if (MU_BUCKET_MASTER != pBucket->m_BucketState) {
        ERROR_LOG("user id %" PRIu64 ", service deny, "
                  "bucket %" PRIu64 " is not master",
                  userId, pBucket->m_BucketId);
        return ReturnStatus(MU_FAILED, MU_SERVICE_DENY);
    }

    return ReturnStatus(MU_SUCCESS);
}

bool
ClientTask::checkPath(const std::string &path)
{
    if (path.empty()) {
        ERROR_LOG("invalid path, empty path");
        return false;
    }

    if ((path.substr(path.length() - 1) == PATH_SEPARATOR_STRING)
        && path != ROOT_PATH) {
        ERROR_LOG("invalid path, %s", path.c_str());
        return false;
    }

    return true;
}

void
ClientTask::logPutDir(MUWorkItem *pItem)
{
    FileOpItem *pRItem = dynamic_cast<FileOpItem *>(pItem);
    Bucket *pBucket =
        BucketManager::getInstance()->get(pRItem->getBucketId());

    assert(pBucket);

    if (NULL == pBucket) {
        ERROR_LOG("no bucket %" PRIu64
                  " exists whilst logging put dir operation, "
                  "path %s",
                  pRItem->getBucketId(), pRItem->getPath().c_str());
        return ;
    }

    LogEvent event;
    event.m_OpCode = MU_OP_PUT_DIR;
    event.m_SeqNr = ++pBucket->m_LogSeq;
    event.m_UserId = pRItem->getUserId();
    event.m_BucketId = pRItem->getBucketId();
    event.m_Token = pRItem->getToken();
    event.m_Path = pRItem->getPath();

    LogDispatcher *pLogDispatcher =
        MURegister::getInstance()->getLogDispatcher();

    assert(pLogDispatcher);

    pLogDispatcher->sendLog(event);
}

void
ClientTask::logDelDir(MUWorkItem *pItem)
{
    FileOpItem *pRItem = dynamic_cast<FileOpItem *>(pItem);
    Bucket *pBucket =
        BucketManager::getInstance()->get(pRItem->getBucketId());

    assert(pBucket);

    if (NULL == pBucket) {
        ERROR_LOG("no bucket %" PRIu64
                  " exists whilst logging del dir operation, "
                  "path %s",
                  pRItem->getBucketId(), pRItem->getPath().c_str());
        return ;
    }

    LogEvent event;
    event.m_OpCode = MU_OP_DEL_DIR;
    event.m_SeqNr = ++pBucket->m_LogSeq;
    event.m_UserId = pRItem->getUserId();
    event.m_BucketId = pRItem->getBucketId();
    event.m_Token = pRItem->getToken();
    event.m_Path = pRItem->getPath();

    LogDispatcher *pLogDispatcher =
        MURegister::getInstance()->getLogDispatcher();

    assert(pLogDispatcher);

    pLogDispatcher->sendLog(event);
}

void
ClientTask::logMovDir(MUWorkItem *pItem)
{
    FileOpItem *pRItem = dynamic_cast<FileOpItem *>(pItem);
    Bucket *pBucket =
        BucketManager::getInstance()->get(pRItem->getBucketId());

    assert(pBucket);

    if (NULL == pBucket) {
        ERROR_LOG("no bucket %" PRIu64
                  " exists whilst logging mov dir operation, "
                  "src path %s, dest path %s",
                  pRItem->getBucketId(),
                  pRItem->getSrcPath().c_str(),
                  pRItem->getDestPath().c_str());
        return ;
    }

    LogEvent event;
    event.m_OpCode = MU_OP_MOV_DIR;
    event.m_SeqNr = ++pBucket->m_LogSeq;
    event.m_UserId = pRItem->getUserId();
    event.m_BucketId = pRItem->getBucketId();
    event.m_Token = pRItem->getToken();
    event.m_SrcPath = pRItem->getSrcPath();
    event.m_DestPath = pRItem->getDestPath();

    LogDispatcher *pLogDispatcher =
        MURegister::getInstance()->getLogDispatcher();

    assert(pLogDispatcher);

    pLogDispatcher->sendLog(event);

}

void
ClientTask::logPutFile(MUWorkItem *pItem)
{
    FileOpItem *pRItem = dynamic_cast<FileOpItem *>(pItem);
    Bucket *pBucket =
        BucketManager::getInstance()->get(pRItem->getBucketId());

    assert(pBucket);

    if (NULL == pBucket) {
        ERROR_LOG("no bucket %" PRIu64
                  " exists whilst logging put file operation, "
                  "path %s",
                  pRItem->getBucketId(), pRItem->getPath().c_str());
        return ;
    }

    LogEvent event;
    event.m_OpCode = MU_OP_PUT_FILE;
    event.m_SeqNr = ++pBucket->m_LogSeq;
    event.m_UserId = pRItem->getUserId();
    event.m_BucketId = pRItem->getBucketId();
    event.m_Token = pRItem->getToken();
    event.m_Path = pRItem->getPath();
    event.m_FileMeta = pRItem->getFileMetaIn();

    LogDispatcher *pLogDispatcher =
        MURegister::getInstance()->getLogDispatcher();

    assert(pLogDispatcher);

    pLogDispatcher->sendLog(event);

}

void
ClientTask::logDelFile(MUWorkItem *pItem)
{
    FileOpItem *pRItem = dynamic_cast<FileOpItem *>(pItem);
    Bucket *pBucket =
        BucketManager::getInstance()->get(pRItem->getBucketId());

    assert(pBucket);

    if (NULL == pBucket) {
        ERROR_LOG("no bucket %" PRIu64
                  " exists whilst logging del file operation, "
                  "path %s",
                  pRItem->getBucketId(), pRItem->getPath().c_str());
        return ;
    }

    LogEvent event;
    event.m_OpCode = MU_OP_DEL_FILE;
    event.m_SeqNr = ++pBucket->m_LogSeq;
    event.m_UserId = pRItem->getUserId();
    event.m_BucketId = pRItem->getBucketId();
    event.m_Token = pRItem->getToken();
    event.m_Path = pRItem->getPath();

    LogDispatcher *pLogDispatcher =
        MURegister::getInstance()->getLogDispatcher();

    assert(pLogDispatcher);

    pLogDispatcher->sendLog(event);

}

void
ClientTask::logMovFile(MUWorkItem *pItem)
{
    FileOpItem *pRItem = dynamic_cast<FileOpItem *>(pItem);
    Bucket *pBucket =
        BucketManager::getInstance()->get(pRItem->getBucketId());

    assert(pBucket);

    if (NULL == pBucket) {
        ERROR_LOG("no bucket %" PRIu64
                  " exists whilst logging mov file operation, "
                  "src path %s, dest path %s",
                  pRItem->getBucketId(),
                  pRItem->getSrcPath().c_str(),
                  pRItem->getDestPath().c_str());
        return ;
    }

    LogEvent event;
    event.m_OpCode = MU_OP_MOV_FILE;
    event.m_SeqNr = ++pBucket->m_LogSeq;
    event.m_UserId = pRItem->getUserId();
    event.m_BucketId = pRItem->getBucketId();
    event.m_Token = pRItem->getToken();
    event.m_SrcPath = pRItem->getSrcPath();
    event.m_DestPath = pRItem->getDestPath();

    LogDispatcher *pLogDispatcher =
        MURegister::getInstance()->getLogDispatcher();

    assert(pLogDispatcher);

    pLogDispatcher->sendLog(event);
}

void
ClientTask::logCreateUser(MUWorkItem *pItem)
{
    UserItem *pRItem = dynamic_cast<UserItem *>(pItem);
    Bucket *pBucket =
        BucketManager::getInstance()->get(pRItem->getBucketId());

    if (NULL == pBucket) {
        ERROR_LOG("no bucket %" PRIu64
                  " exists whilst logging create user operation, "
                  "user id %" PRIu64,
                  pRItem->getBucketId(),
                  pRItem->getUserId());
        return ;
    }

    LogEvent event;
    event.m_OpCode = MU_OP_PUT_USER;
    event.m_SeqNr = ++pBucket->m_LogSeq;
    event.m_UserId = pRItem->getUserId();
    event.m_BucketId = pRItem->getBucketId();

    LogDispatcher *pLogDispatcher =
        MURegister::getInstance()->getLogDispatcher();

    assert(pLogDispatcher);

    pLogDispatcher->sendLog(event);
}

void
ClientTask::logDeleteUser(MUWorkItem *pItem)
{
    UserItem *pRItem = dynamic_cast<UserItem *>(pItem);
    Bucket *pBucket =
        BucketManager::getInstance()->get(pRItem->getBucketId());

    if (NULL == pBucket) {
        ERROR_LOG("no bucket %" PRIu64
                  " exists whilst logging delete user operation, "
                  "user id %" PRIu64,
                  pRItem->getBucketId(),
                  pRItem->getUserId());
        return ;
    }

    LogEvent event;
    event.m_OpCode = MU_OP_DEL_USER;
    event.m_SeqNr = ++pBucket->m_LogSeq;
    event.m_UserId = pRItem->getUserId();
    event.m_BucketId = pRItem->getBucketId();

    LogDispatcher *pLogDispatcher =
        MURegister::getInstance()->getLogDispatcher();

    assert(pLogDispatcher);

    pLogDispatcher->sendLog(event);
}

