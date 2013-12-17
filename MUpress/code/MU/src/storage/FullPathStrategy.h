#ifndef _FULLPATHSTRATEGY_H_
#define _FULLPATHSTRATEGY_H_

#include "BuildStrategy.h"

class FullPathStrategy : public BuildStrategy
{
public:
	FullPathStrategy(string path);
	~FullPathStrategy();
};


#endif
