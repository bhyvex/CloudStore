#ifndef _STOREENGINE_H_
#define _STOREENGINE_H_


#include <iostream>
#include <string>
#include <stdlib.h>
#include "leveldb/db.h"
#include "leveldb/write_batch.h"
#include "leveldb/comparator.h"

using namespace std;

//range [start,limit)

struct RangeStruct
{
	string start;
	string limit;
	void *iterator;
};

struct KeyValuePair;
class StoreEngine
{
public:
	StoreEngine(string path);
	~StoreEngine();

	virtual bool Open(string dbPath) = 0;
	virtual bool Put(string key, string value) = 0;
	virtual bool Get(string key, string &value) = 0;
	virtual bool Delete(string key) = 0;
	virtual RangeStruct RangeOpen(string start, string limit) = 0;
	virtual bool Next(RangeStruct *rs, KeyValuePair *kv) = 0;

	string m_DbPath;
};


#endif
