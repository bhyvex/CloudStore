#include "FSNameSpace.h"
#include "frame/MUMacros.h"

FSNameSpace::FSNameSpace()
{
}

FSNameSpace::~FSNameSpace()
{
}


//file
int FSNameSpace::Open(const char *pathname, int flags)
{
	string path = m_Root + PATH_SEPARATOR_STRING + pathname;
	
	return ::open(path.c_str(), flags);
}
int FSNameSpace::Open(const char *pathname, int flags, mode_t mode)
{
	string path = m_Root + PATH_SEPARATOR_STRING + pathname;
	
	return ::open(path.c_str(), flags, mode);
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
	string path = m_Root + PATH_SEPARATOR_STRING + pathname;
	
	return ::mkdir(path.c_str(), mode);
}
int FSNameSpace::RmDir(const char *pathname)
{
	string path = m_Root + PATH_SEPARATOR_STRING + pathname;

	return ::rmdir(path.c_str());
}

int FSNameSpace::OpenDir(const char *name, Args *args)
{
	string path = m_Root + PATH_SEPARATOR_STRING + name;

	DIR *pDir = ::opendir(path.c_str());

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
	string path = m_Root + PATH_SEPARATOR_STRING + pathname;

	return ::remove(path.c_str());
}

int FSNameSpace::Stat(const char *path, FileAttr *fileAttr)
{
	if(fileAttr == NULL){
		return -1;
	}
	
	struct stat st;
	string pathname = m_Root + PATH_SEPARATOR_STRING + path;
	int ret = stat(pathname.c_str(), &st);
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
	string old_path = m_Root + PATH_SEPARATOR_STRING + oldpath;
	string new_path = m_Root + PATH_SEPARATOR_STRING + newpath;
	return ::rename(old_path.c_str(), new_path.c_str());
}

int FSNameSpace::Link(const char *oldpath, const char *newpath)
{
	string old_path = m_Root + PATH_SEPARATOR_STRING + oldpath;
	string new_path = m_Root + PATH_SEPARATOR_STRING + newpath;
	return ::link(old_path.c_str(), new_path.c_str());
}

int FSNameSpace::Unlink(const char *pathname)
{
	string path = m_Root + PATH_SEPARATOR_STRING + pathname;

	return ::unlink(path.c_str());
}

