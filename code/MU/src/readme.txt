1 为了简化MU的打印，将LogAccessEngine.cpp中的650行
ERROR_LOG("path %s stat() error, %s.", path.c_str(), strerror(errno));
注释掉了


2 为了简化MU的打印，将/oplog/LogRotationTask.cpp中的
INFO_LOG("do rotation for bucket " PRIu64, it->first);
注释掉了



//TODO
3 这个bug还没有修复，这个判断是合理的，但是不是正确的，不一定是sizeof(attr)，有文件片
FileMetaDAO.cpp中的700被注释了，原来的判断根本不对，至于其他问题还没细看

FileMetaDAO.cpp 271行老代码错误，注释掉了

FileMetaDAO.cpp 870行老代码错误，注释掉了



4 请求的pathname的末尾有/无/不统一，会导致leveldb找不到数据，又一个bug
SplitPathStrategy::GetEntry(string pathname, char *buf, int *n) 0
	pathname=bucket1/user1
SplitPathStrategy::GetEntry(string pathname, char *buf, int *n) 1=1
	ParentDir=
SplitPathStrategy::GetEntry(string pathname, char *buf, int *n) 2=1
KVNameSpace::Read(Args *args, void *buf, size_t count) count=56
SplitPathStrategy::GetEntry(string pathname, char *buf, int *n) 0
	pathname=bucket1/user1/
SplitPathStrategy::GetEntry(string pathname, char *buf, int *n) 1=1
	ParentDir=
NotFound: 



5 m_FID的问题曲线救国解决了，使用了一个保存了uint64_t的文件，文件名为fidFile，放在与MU相同的目录下，