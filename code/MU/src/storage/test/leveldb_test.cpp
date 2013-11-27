#include <iostream>
#include <string>
#include <stdlib.h>
#include <string.h>
#include "../LevelDBEngine.h"

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



int main()
{
	LevelDBEngine db;
	string dbPath = "./leveldb_test";
	bool ret;
	
	ret = db.Open(dbPath);
	if(!ret){
		cout <<"db.Open() error"<<endl;
	}

	for(int i = 0; i < 5000; i++){
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

