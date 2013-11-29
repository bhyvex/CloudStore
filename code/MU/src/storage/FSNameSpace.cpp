#include "FSNameSpace.h"

FSNameSpace::FSNameSpace()
{
}

FSNameSpace::~FSNameSpace()
{
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

int FSNameSpace::Stat(const char *path, FileAttr *fileAttr)
{
	if(fileAttr == NULL){
		return -1;
	}
	
	struct stat st;
	int ret = stat(path, &st);
	if(ret == -1){
		return -1;
	}

	fileAttr->m_Mode = st.st_mode;
	fileAttr->m_CTime = st.st_ctime;
    fileAttr->m_MTime = st.st_mtime;
    fileAttr->m_Size = st.st_size;

    return 0;
	
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

