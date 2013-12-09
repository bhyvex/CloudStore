#ifndef _LEVELDBENGINE_H_
#define _LEVELDBENGINE_H_

#include "StoreEngine.h"

#define DBNAME "data.db"


class LevelDBComparator : public leveldb::Comparator 
{
public:
	int Compare(const leveldb::Slice& a, const leveldb::Slice& b) const 
	{
		int num_a = atoi(a.ToString().substr(3).c_str());
		int num_b = atoi(b.ToString().substr(3).c_str());

		if (num_a < num_b) return -1;
		if (num_a > num_b) return +1;

		return 0;
	}

	// Ignore the following methods for now:
	const char* Name() const { return "LevelDBComparator"; }
	void FindShortestSeparator(std::string*, const leveldb::Slice&) const { }
	void FindShortSuccessor(std::string*) const { }
};



struct KeyValuePair;
class LevelDBEngine : public StoreEngine
{
public:
	LevelDBEngine(string path);
	~LevelDBEngine();
	
	virtual bool Open(string dbPath);
	virtual bool Put(string key, string value);
	virtual bool Get(string key, string &value);
	virtual bool Delete(string key);
	virtual RangeStruct RangeOpen(string start, string limit);
	virtual bool Next(RangeStruct *rs, KeyValuePair *kv);

	leveldb::Options m_Options;
	LevelDBComparator m_Cmp;
	leveldb::DB* m_Db;
};


#endif
