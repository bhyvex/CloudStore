1 为了简化MU的打印，将LogAccessEngine.cpp中的650行
ERROR_LOG("path %s stat() error, %s.", path.c_str(), strerror(errno));
注释掉了


2 为了简化MU的打印，将/oplog/LogRotationTask.cpp中的
INFO_LOG("do rotation for bucket " PRIu64, it->first);
注释掉了


3 FileMetaDAO.cpp中的700被注释了，原来的判断根本不对，至于其他问题还没细看