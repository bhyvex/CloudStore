#include <iostream>
#include <string>
#include <stdlib.h>
#include "leveldb/db.h"
#include "leveldb/write_batch.h"
#include "leveldb/comparator.h"



using namespace std;


string IntToString(int num)
{
	int ss;
	char temp[64];
	memset(temp, 0, 64);
	sprintf(temp, "%d", num);
	string s(temp);

	return s;
}

class MyComparator : public leveldb::Comparator {
 public:
  // Three-way comparison function:
  //   if a < b: negative result
  //   if a > b: positive result
  //   else: zero result
  int Compare(const leveldb::Slice& a, const leveldb::Slice& b) const {
	int num_a = atoi(a.ToString().substr(3).c_str());
	int num_b = atoi(b.ToString().substr(3).c_str());
  
	if (num_a < num_b) return -1;
	if (num_a > num_b) return +1;
	
	return 0;
  }

  // Ignore the following methods for now:
  const char* Name() const { return "MyComparator"; }
  void FindShortestSeparator(std::string*, const leveldb::Slice&) const { }
  void FindShortSuccessor(std::string*) const { }
};


class leveldb_engine
{
public:
	string dbPath;
	leveldb::Options dbOptions;
	MyComparator cmp;
	leveldb::DB* db;

	bool Open()
	{
		dbOptions.create_if_missing = true;
		dbOptions.comparator = &cmp;
		leveldb::Status status = leveldb::DB::Open(dbOptions, dbPath.c_str(), &db);
		if (!status.ok()) {
			cerr << status.ToString() << endl;
			return false;
		}else{
			return true;
		}
	}
	bool Put(string key, string value)
	{
		leveldb::Status s = db->Put(leveldb::WriteOptions(), key, value);
		if (!s.ok()) {
			cerr << s.ToString() << endl;
			return false;
		}else{
			return true;
		}
	}
	bool Get(string key, string &value)
	{
		leveldb::Status s = db->Get(leveldb::ReadOptions(), key, &value);
		if (!s.ok()) {
			cerr << s.ToString() << endl;
			return false;
		}else{
			return true;
		}
		
	}

	bool Delete(string key)
	{
		leveldb::Status s = db->Delete(leveldb::WriteOptions(), key);
		if (!s.ok()) {
			cerr << s.ToString() << endl;
			return false;
		}else{
			return true;
		}
	}
	bool Range(string start, string limit)//range [start,limit)
	{
		leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());
		for (it->Seek(start); it->Valid() && it->key().ToString() < limit; it->Next()){
			cout << it->key().ToString() << ": "  << it->value().ToString() << endl;
		}

		delete it;
		
		return true;
	}
};





int main()
{
	leveldb_engine db;
	db.dbPath = "./leveldb_test";
	bool ret;
	
	ret = db.Open();
	if(!ret){
		cout <<"db.Open() error"<<endl;
	}

	for(int i = 0; i < 10000; i++){
		//key
		string key = "key";
		string str_i = IntToString(i);
		key += str_i;
		//value
		string value = "value";
		value += str_i;

		if(i%1000 == 0){
			cout <<key<<endl;
		}
		
		db.Put(key, value);
		if(!ret){
			cout <<"db.Put() error"<<endl;
		}
	}

	db.Range("key100", "key200");


	//atomic
	/*
	std::string value;
	leveldb::Status s = db->Get(leveldb::ReadOptions(), key1, &value);
	if (s.ok()) {
		leveldb::WriteBatch batch;
		batch.Delete(key1);
		batch.Put(key2, value);
		s = db->Write(leveldb::WriteOptions(), &batch);
	}
	*/

	//iterator
	/*
	leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());
	for (it->SeekToFirst(); it->Valid(); it->Next()) {
		cout << it->key().ToString() << ": "  << it->value().ToString() << endl;
		
	}
	assert(it->status().ok());  // Check for any errors found during the scan
	*/
	
	
}
