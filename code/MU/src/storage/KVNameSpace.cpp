#include "KVNameSpace.h"
#include "LevelDBEngine.h"
#include "SplitPathStrategy.h"

KVNameSpace::KVNameSpace()
{
	m_BuildStrategy = new SplitPathStrategy();
}

KVNameSpace::~KVNameSpace()
{
}

//file
Args KVNameSpace::Open(const char *pathname, int flags)
{
	Args args;

	return args;

}

Args KVNameSpace::Open(const char *pathname, int flags, mode_t mode)
{
	Args arg;
	return arg;
}

int KVNameSpace::Close(Args *args)
{
	return 0;
}

int KVNameSpace::Read(Args *args, void *buf, size_t count)
{
	return 0;
}

int KVNameSpace::Write(Args *args, const void *buf, size_t count)
{
	return 0;
}

off_t KVNameSpace::Lseek(Args *args, off_t offset, int whence)
{
	return 0;
}

ssize_t KVNameSpace::readn(Args *args, void *vptr, size_t n)
{
	return 0;
}

ssize_t KVNameSpace::writen(Args *args, const void *vptr, size_t n)
{
	return 0;
}


//dir
int KVNameSpace::MkDir(const char *pathname, mode_t mode)
{
	return 0;
}

int KVNameSpace::RmDir(const char *pathname)
{
	return 0;
}

int KVNameSpace::OpenDir(const char *name, Args *args)
{
	return 0;
}

bool KVNameSpace::ReadDirNext(Args *Dir, Dirent *dirent_)
{
	return false;
}



//common
int KVNameSpace::Remove(const char *pathname)
{
	return 0;
}

int KVNameSpace::Stat(const char *path, FileAttr  *fileAttr)
{
	return 0;
}

int KVNameSpace::Move(const char *oldpath, const char *newpath)
{
	return 0;
}

int KVNameSpace::Link(const char *oldpath, const char *newpath)
{
	return 0;
}

int KVNameSpace::Unlink(const char *pathname)
{
	return 0;
}

int KVNameSpace::RmdirRecursive(const char *pathname)
{
	return 0;
}

