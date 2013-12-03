#ifndef _SPLITPATHSTRATEGY_H_
#define _SPLITPATHSTRATEGY_H_

#include "BuildStrategy.h"

class SplitPathStrategy : public BuildStrategy
{
public:
	SplitPathStrategy();
	~SplitPathStrategy();

	long m_Fid;//gcc atomic sync

	int PutEntry(string pathname, const char* buf, int n);
	
};


#endif
