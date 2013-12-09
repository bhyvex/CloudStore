#ifndef _SPLITPATHSTRATEGY_H_
#define _SPLITPATHSTRATEGY_H_

#include "BuildStrategy.h"
#include "LevelDBEngine.h"
#include <inttypes.h>
#include <string>

using namespace std;


class SplitPathStrategy : public BuildStrategy
{
public:
	SplitPathStrategy();
	~SplitPathStrategy();

	uint64_t m_Fid;//gcc atomic sync

	virtual int PutEntry(string pathname, const char* buf, int n);
	virtual int GetEntry(string pathname, char *buf, int *n);
	virtual int DeleteEntry(string pathname);
	virtual bool FindEntryID(string pathname, string userID, string &fid);
	virtual RangeStruct DirOpen(string pathname);
	virtual bool Next(RangeStruct *rs, KeyValuePair *kv);
	
};


#endif
