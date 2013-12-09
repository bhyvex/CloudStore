#include "BuildStrategy.h"
#include "StoreEngine.h"
#include "LevelDBEngine.h"

BuildStrategy::BuildStrategy(string path)
{
	m_Root = path;
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



