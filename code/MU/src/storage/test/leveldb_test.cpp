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

struct KeyValuePair
{
	string key;
	string value;
};

int main()
{
	LevelDBEngine db(".");
	bool ret;

	for(int i = 0; i < 500; i++){
		//key
		string key = "1/1/";
		string str_i = IntToString(i);
		key += str_i;
		//value
		string value = "value";
		value += str_i;

		//if(i%1000 == 0){
		if(1){
			cout <<key<<endl;
		}
		
		ret = db.Put(key, value);
		if(!ret){
			cout <<"db.Put() error"<<endl;
		}
	}
	cout <<"----------------"<<endl;

	KeyValuePair kv;
	RangeStruct rs;
	rs.start = "1/1/100";
	rs.limit = "1/1/105";
	rs.iterator = NULL;
	cout <<"1"<<endl;
	ret = db.RangeOpen(&rs);
	cout <<"2"<<endl;
	while(db.Next(&rs, &kv)){
		cout <<"3"<<endl;
		cout << kv.key << ": "  << kv.value << endl;
		cout <<"4"<<endl;
	}
	cout <<"5"<<endl;
	
	cout <<"----------------"<<endl;
	
	db.Range("1/1/100", "1/1/105");

/*
	leveldb::Iterator* it = db.m_Db->NewIterator(leveldb::ReadOptions());
	for (; it->Valid(); it->Next()){
		cout << it->key().ToString() << ": "  << it->value().ToString() << endl;
	}
	*/


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

