#ifndef _KVNAMESPACE_H_
#define _KVNAMESPACE_H_

#include "NameSpace.h"
#include "iostream"

class BuildStrategy;

class KVNameSpace : public NameSpace
{
public:
	KVNameSpace(string path);
	~KVNameSpace();
	

	//file
	virtual Args Open(const char *pathname, int flags);
	virtual Args Open(const char *pathname, int flags, mode_t mode);
	virtual int Close(Args *args);
	virtual int Read(Args *args, void *buf, size_t count);
	virtual int Write(Args *args, const void *buf, size_t count);
	virtual off_t Lseek(Args *args, off_t offset, int whence);
	virtual ssize_t readn(Args *args, void *vptr, size_t n);
	virtual ssize_t writen(Args *args, const void *vptr, size_t n);

	//dir
	virtual int MkDir(const char *pathname, mode_t mode);
	virtual int RmDir(const char *pathname);
	virtual int OpenDir(const char *name, Args *args);
	virtual bool ReadDirNext(Args *Dir, Dirent *dirent_);


	//common
	virtual int Remove(const char *pathname);
	virtual int Stat(const char *path, FileAttr  *fileAttr);
	virtual int Move(const char *oldpath, const char *newpath);
	virtual int Link(const char *oldpath, const char *newpath);
	virtual int Unlink(const char *pathname);
	virtual int RmdirRecursive(const char *pathname);

private:
	BuildStrategy *m_BuildStrategy;

};

#endif
