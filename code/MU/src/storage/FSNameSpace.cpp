#include "FSNameSpace.h"
#include "frame/MUMacros.h"
#include "frame/ReturnStatus.h"
#include "protocol/MUMacros.h"
#include "log/log.h"

#include <errno.h>




FSNameSpace::FSNameSpace()
{
}

FSNameSpace::~FSNameSpace()
{
}


//file
Args FSNameSpace::Open(const char *pathname, int flags)
{
	Args args;
	args.valid = false;
	string path = m_Root + PATH_SEPARATOR_STRING + pathname;
	
	int fd = ::open(path.c_str(), flags);
	args.arg2 = fd;
	if(fd >= 0){
		args.valid = true;
	}

	return args;
}
Args FSNameSpace::Open(const char *pathname, int flags, mode_t mode)
{
	Args args;
	args.valid = false;
	string path = m_Root + PATH_SEPARATOR_STRING + pathname;
	
	int fd = ::open(path.c_str(), flags, mode);
	args.arg2 = fd;
	if(fd >= 0){
		args.valid = true;
	}

	return args;
}
int FSNameSpace::Close(Args *args)
{
	return ::close(args->arg2);
}
int FSNameSpace::Read(Args *args, void *buf, size_t count)
{
	return ::read(args->arg2, buf, count);
}
int FSNameSpace::Write(Args *args, const void *buf, size_t count)
{
	return ::write(args->arg2, buf, count);
}
off_t FSNameSpace::Lseek(Args *args, off_t offset, int whence)
{
	return ::lseek(args->arg2, offset, whence);
}
ssize_t FSNameSpace::readn(Args *args, void *vptr, size_t n)
{
    if (NULL == vptr) {
        DEBUG_LOG("NULL pointer.");
        return -1;
    }

    size_t nleft;
    ssize_t nread;
    char *ptr = NULL;

    nleft = n;
    nread = 0;
    ptr = static_cast<char *>(vptr);

    while (nleft > 0) {
        if ((nread = ::read(args->arg2, ptr, nleft)) < 0) {
            if (errno == EINTR) {
                nread = 0;
                continue;

            } else {
                DEBUG_LOG("Syscall Error: read. errno %d, %s.",
                          errno, strerror(errno));
                return -1;
            }

        } else if (nread == 0) {
            break;  // EOF
        }

        nleft -= nread;
        ptr += nread;
    }

    return (n - nleft);
}
ssize_t FSNameSpace::writen(Args *args, const void *vptr, size_t n)
{
    if (NULL == vptr) {
        DEBUG_LOG("NULL pointer.");
        return -1;
    }

    size_t nleft;
    ssize_t nwritten;
    char *ptr = NULL;

    nleft = n;
    nwritten = 0;
    ptr = static_cast<char *>(const_cast<void *>(vptr));

    while (nleft > 0) {
        if ((nwritten = ::write(args->arg2, ptr, nleft)) < 0) {
            if (errno == EINTR) {
                nwritten = 0;
                continue;

            } else {
                if (EAGAIN != errno) {
                    DEBUG_LOG("Syscall Error: write. errno %d, %s",
                              errno, strerror(errno));
                }

                return -1;
            }

        } else if (nwritten == 0) {
            break;
        }

        nleft -= nwritten;
        ptr += nwritten;
    }

    return (n - nleft);
}




//dir
int FSNameSpace::MkDir(const char *pathname, mode_t mode)
{
	string path = m_Root + PATH_SEPARATOR_STRING + pathname;
	cout <<"path="<<path<<endl;
	
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
	args->valid = false;

	DIR *pDir = ::opendir(path.c_str());

	(args->arg1) = pDir;
	
	if(NULL == args->arg1){
		return -1;
	}else{
		args->valid = true;
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
		dirent_->filetype = MU_DIRECTORY;
	}else{
		dirent_->filetype = MU_REGULAR_FILE;
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

    if(S_ISDIR(st.st_mode)){
    	fileAttr->m_Type = MU_DIRECTORY;
    }else{
    	fileAttr->m_Type = MU_REGULAR_FILE;
    }
    

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




int FSNameSpace::RmdirRecursive(const char *pathname)
{
    int rt = 0;
    std::string entryName;
    std::string npath;

    string path = m_Root + PATH_SEPARATOR_STRING + pathname;

    DIR *pDir = ::opendir(path.c_str());

    if (NULL == pDir) {
        DEBUG_LOG("path %s, opendir() error, %s.",
                  path.c_str(), strerror(errno));

        return -1;
    }

    // delete its children

    struct dirent *pEnt = NULL;

    while (NULL != (pEnt = ::readdir(pDir))) {
        entryName = pEnt->d_name;

        // omit "." and ".."
        if (entryName == DENTRY_CURRENT_DIR
            || entryName == DENTRY_PARENT_DIR) {
            //|| entryName == USER_INFO_FILE_NAME) {
            continue;
        }

        npath = std::string(pathname) + PATH_SEPARATOR_STRING + entryName;

        if (DT_DIR == pEnt->d_type) {
            // directory, call myself to delete it
            rt = RmdirRecursive(npath.c_str());

            if (rt < 0) {
                DEBUG_LOG("path %s, rmdirRecursive() error", npath.c_str());
                return rt;
            }

        } else {
            // delete it directly
            rt = ::unlink(npath.c_str());

            if (-1 == rt) {
                DEBUG_LOG("path %s, unlink() error, %s.",
                          npath.c_str(), strerror(errno));
                return -1;
            }
        }
    }

    ::closedir(pDir);

    // delete path

    rt = ::rmdir(path.c_str());

    if (-1 == rt) {
        DEBUG_LOG("path %s, rmdir() error, %s.",
                  path.c_str(), strerror(errno));
        return -1;
    }

    return 0;
}


