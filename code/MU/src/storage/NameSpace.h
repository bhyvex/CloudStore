#ifndef _NAMESPACE_H_
#define _NAMESPACE_H_


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <iostream>
#include <string>

using namespace std;

enum FileType
{
	DIR_,
	FILE_
};

struct Args
{
	void *arg1;
	void *arg2;
};

struct Dirent
{
	string filename;
	enum FileType filetype;
};

class NameSpace 
{
public:
	NameSpace();
	~NameSpace();

	/* interface like posix */
	//file
	virtual int Open(const char *pathname, int flags) = 0;
	virtual int Open(const char *pathname, int flags, mode_t mode) = 0;
	virtual int Close(int fd) = 0;
	virtual int Read(int fd, void *buf, size_t count) = 0;
	virtual int Write(int fd, const void *buf, size_t count) = 0;

	//dir
	virtual int MkDir(const char *pathname, mode_t mode) = 0;
	virtual int RmDir(const char *pathname) = 0;
	virtual int OpenDir(const char *name, Args *args) = 0;
	virtual bool ReadDirNext(Args *Dir, Dirent *dirent_)= 0;


	//common
	virtual int Remove(const char *pathname) = 0;
	virtual int Stat(const char *path, Args *args) = 0;
	virtual int Move(const char *oldpath, const char *newpath) = 0;
	virtual int Link(const char *oldpath, const char *newpath) = 0;
	virtual int Unlink(const char *pathname) = 0;


};


#endif
