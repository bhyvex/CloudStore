#include "Channel.h"
#include "NameSpace.h"
#include "FSNameSpace.h"
#include "KVNameSpace.h"
#include <sys/statvfs.h>
#include <errno.h>
#include <string.h>
#include "log/log.h"





Channel::Channel()
{
}

Channel::~Channel()
{

}

void Channel::setID(int ID)
{
	m_Id = ID;
}

bool Channel::setRoot(string path)
{
	if(1){
		m_Root = path;
		return true;
	}else{
		return false;
	}
}

bool Channel::setDataNS(enum NameSpaceType NST)
{
	switch(NST){
		case FS:
			m_DataNS = new FSNameSpace(m_Root);
			m_DataNS->setRoot(m_Root);
			break;
		case KV:
			m_DataNS = new KVNameSpace(m_Root);
			m_DataNS->setRoot(m_Root);
			break;
	}

	return true;
}

bool Channel::setSizeInMB()
{
	struct statvfs st;
	int rt = ::statvfs(m_Root.c_str(),&st);

	if (-1 == rt) {
		ERROR_LOG("statvfs() error, %s.", strerror(errno));
		return false;
	}

	m_SizeInMB = ((uint64_t) st.f_frsize) * st.f_blocks / 1024 / 1024;

	return true;
}


