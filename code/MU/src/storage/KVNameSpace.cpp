#include "KVNameSpace.h"
#include "LevelDBEngine.h"
#include "SplitPathStrategy.h"
#include "protocol/MUMacros.h"
#include "Key.h"
#include "KeyValuePair.h"

KVNameSpace::KVNameSpace(string path):
	NameSpace(path)
{
	m_BuildStrategy = new SplitPathStrategy(path);
}

KVNameSpace::~KVNameSpace()
{
}

//file open
Args KVNameSpace::Open(const char *pathname, int flags)
{
	Args args;
	args.valid = false;
	
	if(pathname == NULL){
		cout <<"KVNameSpace::Open() pathname is NULL"<<endl;
		return args;
	}
	
	args.arg3 = string(pathname);
	args.valid = true;
	return args;

}
//file create
Args KVNameSpace::Open(const char *pathname, int flags, mode_t mode)
{
	Args args;
	args.valid = false;

	if(pathname == NULL){
		cout <<"KVNameSpace::Open() pathname is NULL"<<endl;
		return args;
	}
	
	args.arg3 = string(pathname);
	args.valid = true;
	return args;
}

int KVNameSpace::Close(Args *args)
{
	if(args == NULL){
		cout <<"KVNameSpace::Close() args is NULL"<<endl;
		return -1;
	}
	
	args->valid = true;
	return 0;
}

int KVNameSpace::Read(Args *args, void *buf, size_t count)
{
	int ret;
	args->valid = false;

	if(args == NULL){
		cout <<"KVNameSpace::Read() args is NULL"<<endl;
		return -1;
	}

	if(buf == NULL){
		cout <<"KVNameSpace::Read() buf is NULL"<<endl;
		return -1;
	}
	
	m_BuildStrategy->GetEntry(args->arg3, (char *)buf, &ret);
	cout <<"KVNameSpace::Read(Args *args, void *buf, size_t count) count="<<ret<<endl;
	args->valid = true;
	return ret;
}

int KVNameSpace::Write(Args *args, const void *buf, size_t count)
{
	int ret = count;
	args->valid = false;

	if(args == NULL){
		cout <<"KVNameSpace::Write() args is NULL"<<endl;
		return -1;
	}

	if(buf == NULL){
		cout <<"KVNameSpace::Write() buf is NULL"<<endl;
		return -1;
	}
	
	m_BuildStrategy->PutEntry(args->arg3, (char *)buf, count);
	args->valid = true;
	
	return ret;
}

off_t KVNameSpace::Lseek(Args *args, off_t offset, int whence)
{
	return 0;
}

ssize_t KVNameSpace::readn(Args *args, void *vptr, size_t n)
{
	return Read(args, vptr, n);
}

ssize_t KVNameSpace::writen(Args *args, const void *vptr, size_t n)
{
	return Write(args, vptr, n);
}


//dir
int KVNameSpace::MkDir(const char *pathname, mode_t mode)
{
	int ret;
	FileAttr st;
	st.m_FID = __sync_add_and_fetch(&(m_BuildStrategy->m_Fid), 1);
	st.m_CTime = 0;
	st.m_Mode = 0;
	st.m_MTime = 0;
	st.m_Size = 0;
	st.m_Type = MU_DIRECTORY;
	st.m_Version = 0;

	ret = m_BuildStrategy->PutEntry(pathname, (const char*)&st, sizeof(st));
	
	return ret;
}

int KVNameSpace::RmDir(const char *pathname)
{
	int ret = m_BuildStrategy->DeleteEntry(pathname);
	return ret;
}

int KVNameSpace::OpenDir(const char *name, Args *args)
{
	RangeStruct rs;
	RangeStruct *n_rs= new RangeStruct();
	rs = m_BuildStrategy->DirOpen(name);
	*n_rs = rs;
	args->arg1 = n_rs;
	args->valid = true;

	return 0;
}

bool KVNameSpace::ReadDirNext(Args *Dir, Dirent *dirent_)
{
	bool ret = false;
	RangeStruct *rs = (RangeStruct *)(Dir->arg1);
	KeyValuePair kv;
	
	ret = m_BuildStrategy->Next(rs, &kv);

	//key = "key=5/6/filename"
	//value = "FileAttr";
	size_t pos;
	pos = kv.key.find(KEY_SEPARATOR);
	kv.key = kv.key.substr(pos+1);
	pos = kv.key.find(KEY_SEPARATOR);
	kv.key = kv.key.substr(pos+1);

	FileAttr fa;
	memcpy(&fa, kv.value.data(), sizeof(fa));

	dirent_->filename = kv.key;
	dirent_->filetype = fa.m_Type;

	return ret;
}



//common
int KVNameSpace::Remove(const char *pathname)
{
	int ret = m_BuildStrategy->DeleteEntry(pathname);
	return ret;
}

int KVNameSpace::Stat(const char *path, FileAttr  *fileAttr)
{
	Args args;
	args.arg3 = path;
	int ret = Read(&args, fileAttr, sizeof(FileAttr));
	if(ret < 0){
		return -1;
	}else{
		return 0;
	}
}

//TODO
int KVNameSpace::Move(const char *oldpath, const char *newpath)
{
	return 0;
}

//TODO
int KVNameSpace::Link(const char *oldpath, const char *newpath)
{
	return 0;
}

//TODO
int KVNameSpace::Unlink(const char *pathname)
{
	return 0;
}

//TODO
int KVNameSpace::RmdirRecursive(const char *pathname)
{
	return 0;
}

