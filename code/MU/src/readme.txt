1 Ϊ�˼�MU�Ĵ�ӡ����LogAccessEngine.cpp�е�650��
ERROR_LOG("path %s stat() error, %s.", path.c_str(), strerror(errno));
ע�͵���


2 Ϊ�˼�MU�Ĵ�ӡ����/oplog/LogRotationTask.cpp�е�
INFO_LOG("do rotation for bucket " PRIu64, it->first);
ע�͵���



//TODO
3 ���bug��û���޸�������ж��Ǻ���ģ����ǲ�����ȷ�ģ���һ����sizeof(attr)�����ļ�Ƭ
FileMetaDAO.cpp�е�700��ע���ˣ�ԭ�����жϸ������ԣ������������⻹ûϸ��

FileMetaDAO.cpp 271���ϴ������ע�͵���

FileMetaDAO.cpp 870���ϴ������ע�͵���



4 �����pathname��ĩβ��/��/��ͳһ���ᵼ��leveldb�Ҳ������ݣ���һ��bug
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



5 m_FID���������߾ȹ�����ˣ�ʹ����һ��������uint64_t���ļ����ļ���ΪfidFile��������MU��ͬ��Ŀ¼�£�