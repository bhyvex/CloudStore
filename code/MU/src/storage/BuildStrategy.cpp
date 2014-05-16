#include "BuildStrategy.h"
#include "StoreEngine.h"
#include "LevelDBEngine.h"
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


BuildStrategy::BuildStrategy(string path)
{
	m_Root = path;

	//-------------------
	/*
	if ((m_FidFd = open("fidFile", O_RDWR| O_CREAT, S_IRWXU)) < 0){ 
		perror("open() ");
	}

	void* sfp;
	if((sfp = mmap(NULL, sizeof(*m_pFid), PROT_READ|PROT_WRITE,MAP_SHARED, m_FidFd, 0)) == NULL){
		perror("mmap() ");
	}

	m_pFid = (uint64_t*)sfp;
	*/

	//---------------------


	
	m_Fid = 0;
	m_StoreEngine = NULL;
}

BuildStrategy::~BuildStrategy()
{
}


bool BuildStrategy::setRoot(string pathname)
{
	m_Root = pathname;

	return true;
}



