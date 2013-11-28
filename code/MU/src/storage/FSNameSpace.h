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
	virtual int Open(const char *pathname, int flags);//
	virtual int Close(int fd);//
	virtual int Read(int fd, void *buf, size_t count);//
	virtual int Write(int fd, const void *buf, size_t count);//

	//dir
	virtual int MkDir(const char *pathname, mode_t mode);//
	virtual int RmDir(const char *pathname);//
	virtual int ReadDir(const char *name);//
	virtual Args ReadDirNext();//


	//common
	virtual int Remove(const char *pathname);//
	virtual int Stat(const char *path, Args args);
	virtual int Move(const char *oldpath, const char *newpath);
	virtual int Link(const char *oldpath, const char *newpath);
	virtual int Unlink(const char *pathname);


	string m_Root;
	DIR *m_pDir;
	


	
};


#endif
