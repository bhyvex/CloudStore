#ifndef _BUILDSTRATEGY_H_
#define _BUILDSTRATEGY_H_

#include <iostream>

using namespace std;


class StoreEngine;

class BuildStrategy
{
public:
	BuildStrategy();
	~BuildStrategy();

	virtual int PutEntry(string pathname, const char* buf, int n) = 0;
	virtual int GetEntry(string pathname, char *buf, int *n) = 0;
	virtual int DeleteEntry(string pathname) = 0;
	virtual bool FindEntryID(string pathname, string userID, string &fid) = 0;
	

	StoreEngine *m_StoreEngine;
};


#endif
