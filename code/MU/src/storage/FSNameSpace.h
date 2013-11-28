#ifndef _FSNAMESPACE_H_
#define _FSNAMESPACE_H_

#include <iostram>
#include <string>
#include "NameSpace.h"


using namespace std;


class FSNameSpace : public NameSpace
{
public:
	FSNameSpace();
	~FSNameSpace();

	bool setRoot();

	//file
	virtual int Open(const char *pathname, int flags);
	virtual int Close(int fd);
	virtual int Read(int fd, void *buf, size_t count);
	virtual int Write(int fd, const void *buf, size_t count);

	//dir
	virtual int MkDir(const char *pathname, mode_t mode);
	virtual int RmDir(const char *pathname);
	virtual struct DIR* OpenDir(const char *name);
	virtual int CloseDir(DIR *dirp);
	virtual struct dirent* ReadDir(DIR *dirp);


	//common
	virtual int Remove(const char *pathname);
	virtual int Stat(const char *path, struct stat *buf);
	virtual int Move(const char *oldpath, const char *newpath);
	virtual int Link(const char *oldpath, const char *newpath);
	virtual int Unlink(const char *pathname);


	string m_Root;


	
};


#endif
