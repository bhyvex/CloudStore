#ifndef _BUILDSTRATEGY_H_
#define _BUILDSTRATEGY_H_

#include <iostream>
#include "StoreEngine.h"


using namespace std;


class StoreEngine;

class BuildStrategy
{
public:
	BuildStrategy(string path);
	~BuildStrategy();

	bool setRoot(string pathname);

	virtual int PutEntry(string pathname, const char* buf, int n) = 0;
	virtual int GetEntry(string pathname, char *buf, int *n) = 0;
	virtual int DeleteEntry(string pathname) = 0;
	virtual bool FindEntryID(string pathname, string userID, string &fid) = 0;
	//virtual RangeStruct DirOpen(string pathname) = 0;
	virtual bool DirOpen(string pathname, RangeStruct *rs) = 0;
	virtual bool Next(RangeStruct *rs, KeyValuePair *kv) = 0;

	uint64_t m_Fid;//gcc atomic sync
	StoreEngine *m_StoreEngine;
	string m_Root;
};


#endif
