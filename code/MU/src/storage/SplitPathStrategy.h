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
	SplitPathStrategy(string path);
	~SplitPathStrategy();

	virtual int PutEntry(string pathname, const char* buf, int n);
	virtual int GetEntry(string pathname, char *buf, int *n);
	virtual int DeleteEntry(string pathname);
	virtual bool FindEntryID(string pathname, string userID, string &fid);
	//virtual RangeStruct DirOpen(string pathname);
	virtual bool DirOpen(string pathname, RangeStruct *rs);
	virtual bool Next(RangeStruct *rs, KeyValuePair *kv);
	
};


#endif
