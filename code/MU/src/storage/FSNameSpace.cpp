#include "FSNameSpace.h"

FSNameSpace::FSNameSpace()
{
}

FSNameSpace::~FSNameSpace()
{
}


bool FSNameSpace::setRoot(string path)
{
	if(1){
		m_Root= path;
		return true;
	}else{
		return false;
	}
}


//file
int FSNameSpace::Open(const char *pathname, int flags)
{
	return ::open(pathname, flags);
}
int FSNameSpace::Close(int fd)
{
	return ::close(fd);
}
int FSNameSpace::Read(int fd, void *buf, size_t count)
{
	return ::read(fd, buf, count);
}
int FSNameSpace::Write(int fd, const void *buf, size_t count)
{
	return ::write(fd, buf, count);
}


//dir
int FSNameSpace::MkDir(const char *pathname, mode_t mode)
{
	return ::mkdir(pathname, mode);
}
int FSNameSpace::RmDir(const char *pathname)
{
	return ::rmdir(pathname);
}
int FSNameSpace::ReadDir(const char *name, vector<KeyValuePair> *KVVec)
{
	
	
	while (NULL != (pEnt = ::readdir(pDir))) {
		entryName = pEnt->d_name;
		
		KVVec->push_back();
	}
}

int FSNameSpace::ReadDir(const char *name)
{
	int ret;
		
	m_pDir = ::opendir(name);
	if(NULL == m_pDir){
		return -1;
	}else{
		return 0;
	}
}

Args FSNameSpace::ReadDirNext()
{
	int ret;
	struct dirent *pEnt = NULL;
	Args args;

	args.arg1 = pEnt;
	
	if (NULL == (pEnt = ::readdir(m_pDir))) {
		::closedir(m_pDir);
		m_pDir = NULL;
	}

	return args;
}


//common
int FSNameSpace::Remove(const char *pathname)
{
	return ::remove(pathname);
}

int FSNameSpace::Stat(const char *path, Args args)
{
	return ::stat(path, args.arg1);
}

int FSNameSpace::Move(const char *oldpath, const char *newpath)
{
	return ::rename(oldpath, newpath);
}

int FSNameSpace::Link(const char *oldpath, const char *newpath)
{
	return ::link(oldpath, newpath);
}

int FSNameSpace::Unlink(const char *pathname)
{
	return ::unlink(pathname);
}

