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
int FSNameSpace::Open(const char *pathname, int flags, mode_t mode)
{
	return ::open(pathname, flags, mode);
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

int FSNameSpace::OpenDir(const char *name, Args *args)
{
	DIR *pDir = ::opendir(name);

	(args->arg1) = pDir;
	
	if(NULL == args->arg1){
		return -1;
	}else{
		return 0;
	}
}

bool FSNameSpace::ReadDirNext(Args *Dir, Dirent *dirent_)
{
	DIR *pDir = (DIR*)(Dir->arg1);
	
	struct dirent *pEnt = NULL;
	
	if (NULL == (pEnt = ::readdir(pDir))) {
		::closedir(pDir);
		return false;
	}

	dirent_->filename = pEnt->d_name;
	
	if(pEnt->d_type == DT_DIR){
		dirent_->filetype = DIR_;
	}else{
		dirent_->filetype = FILE_;
	}

	return true;
}


//common
int FSNameSpace::Remove(const char *pathname)
{
	return ::remove(pathname);
}

int FSNameSpace::Stat(const char *path, Args *args)
{
	return ::stat(path, (struct stat*)(args->arg1));
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

