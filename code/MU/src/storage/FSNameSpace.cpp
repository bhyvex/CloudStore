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
virtual int RmDir(const char *pathname)
{
}
virtual struct DIR* OpenDir(const char *name)
{
}
virtual int CloseDir(DIR *dirp)
{
}
virtual struct dirent* ReadDir(DIR *dirp)
{
}


//common
int FSNameSpace::Remove(const char *pathname);
int FSNameSpace::Stat(const char *path, struct stat *buf);
int FSNameSpace::Move(const char *oldpath, const char *newpath);
int FSNameSpace::Link(const char *oldpath, const char *newpath);
int FSNameSpace::Unlink(const char *pathname);

