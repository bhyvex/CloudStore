#ifndef _NAMESPACE_H_
#define _NAMESPACE_H_


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <iostream>
#include <string>
#include "data/FileMeta.h"

using namespace std;


struct Args
{
	void *arg1;
	int arg2;
	string arg3;
	bool valid;
};

struct Dirent
{
	string filename;
	uint32_t filetype;
};

enum NameSpaceType
{
	FS,
	KV
};

class NameSpace 
{
public:
	NameSpace(string path);
	~NameSpace();

	/*
	  * @path must be absolute path and the last letter mustn't be '/'
	  * so eg: 
	  *		@path == "/home/cstore/MU/data" is right;
	  *		@path == "/home/cstore/MU/data/" is wrong.
	  */
	bool setRoot(string path);

	/* interface like posix, but the first letter mustn't be "/"
	  *
	  * so eg :
	  *		@path == "bucket0/user1" is right;
	  *		@path == "/bucket0/user1" is wrong.
	  */
	//file
	virtual Args Open(const char *pathname, int flags) = 0;
	virtual Args Open(const char *pathname, int flags, mode_t mode) = 0;
	virtual int Close(Args *args) = 0;
	virtual int Read(Args *args, void *buf, size_t count) = 0;
	virtual int Write(Args *args, const void *buf, size_t count) = 0;
	virtual off_t Lseek(Args *args, off_t offset, int whence) = 0;
	virtual ssize_t readn(Args *args, void *vptr, size_t n) = 0;
	virtual ssize_t writen(Args *args, const void *vptr, size_t n) = 0;

	//dir
	virtual int MkDir(const char *pathname, mode_t mode) = 0;
	virtual int RmDir(const char *pathname) = 0;
	virtual int OpenDir(const char *name, Args *args) = 0;
	virtual bool ReadDirNext(Args *Dir, Dirent *dirent_)= 0;


	//common
	virtual int Remove(const char *pathname) = 0;
	virtual int Stat(const char *path, FileAttr *fileAttr) = 0;
	virtual int Move(const char *oldpath, const char *newpath) = 0;
	virtual int Link(const char *oldpath, const char *newpath) = 0;
	virtual int Unlink(const char *pathname) = 0;
	virtual int RmdirRecursive(const char *pathname) = 0;


	
	string m_Root;


};


#endif
