/*
 * @file ClientTask.h
 * @brief Task for client requests processing.
 *
 * @version 1.0
 * @date Mon Jul  2 19:48:36 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#ifndef ClientTask_H_
#define ClientTask_H_

#include "frame/PassiveTask.h"
#include "frame/MUTCPAgent.h"
#include "frame/MUWorkItem.h"

class ClientTask : public PassiveTask
{
public:
    virtual ~ClientTask();

    ClientTask(MUTask *pParent);

    int next(MUTCPAgent *pAgent, const InReq &req);

    int next(MUWorkItem *pItem);

    void destroy(MUTCPAgent *pChannel);

private:
    enum _WorkItemType {
        FILE_OP_ITEM,
        USER_ITEM,
        LOG_ITEM
    };

    enum _ClientTaskState {
        TASK_INIT,
        TASK_WORKING,
        TASK_FINISH,
        TASK_ERROR,
        TASK_RECYCLED
    };

private:
    /**
     * @brief Dispatch client request to their processor.
     *
     * @param req data packet
     *
     * @return 0 if a success, or -1
     */
    int dispatch(const InReq &req);

    int getUserInfo(const InReq &req);

    int putDir(const InReq &req);

    int delDir(const InReq &req);

    int getDir(const InReq &req);

    int statDir(const InReq &req);

    int getDir2(const InReq &req);

    int movDir(const InReq &req);

    int putFile(const InReq &req);

    int delFile(const InReq &req);

    int getFile(const InReq &req);

    int movFile(const InReq &req);

    int getUserLog(const InReq &req);

    int createUser(const InReq &req);

    int deleteUser(const InReq &req);

    /**
     * @brief Dispatch results of processed request.
     *
     * @param pItem result
     *
     * @return 0 if a success, or -1
     */
    int dispatch(MUWorkItem *pItem);

    int dispatchFileOpItem(MUWorkItem *pItem);

    int dispatchUserItem(MUWorkItem *pItem);

    int dispatchLogItem(MUWorkItem *pItem);

    int getUserInfo(MUWorkItem *pItem);

    int putDir(MUWorkItem *pItem);

    int delDir(MUWorkItem *pItem);

    int getDir(MUWorkItem *pItem);

    int statDir(MUWorkItem *pItem);

    int getDir2(MUWorkItem *pItem);

    int movDir(MUWorkItem *pItem);

    int putFile(MUWorkItem *pItem);

    int delFile(MUWorkItem *pItem);

    int getFile(MUWorkItem *pItem);

    int movFile(MUWorkItem *pItem);

    int getUserLog(MUWorkItem *pItem);

    int createUser(MUWorkItem *pItem);

    int deleteUser(MUWorkItem *pItem);

    void logPutDir(MUWorkItem *pItem);

    void logDelDir(MUWorkItem *pItem);

    void logMovDir(MUWorkItem *pItem);

    void logPutFile(MUWorkItem *pItem);

    void logDelFile(MUWorkItem *pItem);

    void logMovFile(MUWorkItem *pItem);

    void logCreateUser(MUWorkItem *pItem);

    void logDeleteUser(MUWorkItem *pItem);

    /**
     * @brief Response to client when errors occurred.
     *
     * @param cmd Protocol command.
     * @param requestId Client request Id.
     */
    void errorResponse(uint32_t cmd, uint32_t requestId);

    /**
     * @brief Simple response without extra data.
     *
     * @param cmd Protocol command.
     * @param requestId Client request Id.
     * @param error Error code.
     */
    void simpleResponse(uint32_t cmd, uint32_t error, uint32_t requestId);

    /**
     * @brief Validate user requests before processing.
     *
     * @param userId
     *
     * @return
     */
    ReturnStatus checkUser(uint64_t userId);

    /**
     * @brief check if the file path is valid
     *
     * @param path
     *
     * @return
     */
    bool checkPath(const std::string &path);
};

#endif  // ClientTask_H_

