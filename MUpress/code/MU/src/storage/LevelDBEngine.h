#ifndef _LEVELDBENGINE_H_
#define _LEVELDBENGINE_H_

#include "StoreEngine.h"
#include "Key.h"
#include <stdlib.h>

#define DBNAME "data.db"


class LevelDBComparator : public leveldb::Comparator 
{
public:
	int Compare(const leveldb::Slice& a, const leveldb::Slice& b) const 
	{
	/*
		int num_a = atoi(a.ToString().substr(3).c_str());
		int num_b = atoi(b.ToString().substr(3).c_str());

		if (num_a < num_b) return -1;
		if (num_a > num_b) return +1;

		return 0;
		*/


		string str_a = a.ToString();
		string str_b = b.ToString();
		size_t pos;
		
		pos = str_a.find(KEY_SEPARATOR); 
		string userid_a = str_a.substr(0, pos);
		str_a = str_a.substr(pos+1);
		int userid_a_num = atoi(userid_a.c_str());
		
		pos = str_a.find(KEY_SEPARATOR);
		string pid_a = str_a.substr(0, pos);
		string filename_a = str_a.substr(pos+1);
		int pid_a_num = atoi(pid_a.c_str());
		
		pos = str_b.find(KEY_SEPARATOR); 
		string userid_b = str_b.substr(0, pos);
		str_b = str_b.substr(pos+1);
		int userid_b_num = atoi(userid_b.c_str());
		
		pos = str_b.find(KEY_SEPARATOR);
		string pid_b = str_b.substr(0, pos);
		string filename_b = str_b.substr(pos+1);
		int pid_b_num = atoi(pid_b.c_str());
		
		if(userid_a_num < userid_b_num) return -1;
		if(userid_a_num > userid_b_num) return +1;
		//cout <<"userid is equal"<<endl;
		
		if(pid_a_num < pid_b_num) return -1;
		if(pid_a_num > pid_b_num) return +1;
		//cout <<"pid is equal"<<endl;
		
		if(filename_a < filename_b) return -1;
		if(filename_a > filename_b) return +1;
		//cout <<"filename is equal"<<endl;
		
		return 0;

		
	}

	// Ignore the following methods for now:
	const char* Name() const { return "LevelDBComparator"; }
	void FindShortestSeparator(std::string*, const leveldb::Slice&) const { }
	void FindShortSuccessor(std::string*) const { }
};



struct KeyValuePair;
struct RangeStruct;
class LevelDBEngine : public StoreEngine
{
public:
	LevelDBEngine(string path);
	~LevelDBEngine();
	
	virtual bool Open(string dbPath);
	virtual bool Put(string key, string value);
	virtual bool Get(string key, string &value);
	virtual bool Delete(string key);
	virtual bool Range(string start, string limit);
	virtual bool RangeOpen(RangeStruct *rs);
	virtual bool Next(RangeStruct *rs, KeyValuePair *kv);

	leveldb::Options m_Options;
	LevelDBComparator m_Cmp;
	leveldb::DB* m_Db;
};


#endif
