#include "LevelDBEngine.h"
#include "KeyValuePair.h"


LevelDBEngine::LevelDBEngine(string path):
	StoreEngine(path),
	m_Db(NULL)
{

	string dbpath = m_DbPath + "/" + DBNAME;
	cout <<"create dbpath="<<dbpath<<endl;
	bool ret = Open(dbpath);
	if(ret == false){
		cout <<"LevelDB Open error."<<endl;
		exit(1);
	}
	
}


LevelDBEngine::~LevelDBEngine()
{
}


bool LevelDBEngine::Open(string dbPath)
{
	m_DbPath = dbPath;
	
	m_Options.create_if_missing = true;
	m_Options.comparator = &m_Cmp;
	m_Options.write_buffer_size = 100000000;

	
	leveldb::Status status = leveldb::DB::Open(m_Options, m_DbPath.c_str(), &m_Db);
	if (!status.ok()) {
		cerr << status.ToString() << endl;
		return false;
	}else{
		return true;
	}

}

bool LevelDBEngine::Put(string key, string value)
{
	leveldb::Status s = m_Db->Put(leveldb::WriteOptions(), key, value);
	//cout <<"LevelDBEngine::Put()"<<endl;
	//cout <<"	key="<<key<<"  size="<<key.size()<<endl;
	//cout <<"	value="<<value<<endl;
	if (!s.ok()) {
		cout << s.ToString() << endl;
		return false;
	}else{
		return true;
	}

}

bool LevelDBEngine::Get(string key, string &value)
{
	leveldb::Status s = m_Db->Get(leveldb::ReadOptions(), key, &value);
	//cout <<"LevelDBEngine::Get()"<<endl;
	//cout <<"	key="<<key<<"  size="<<key.size()<<endl;
	if (!s.ok()) {
		cerr << s.ToString() << endl;
		return false;
	}else{
		//cout <<"	value="<<value<<endl;
		return true;
	}

}

bool LevelDBEngine::Delete(string key)
{
	leveldb::Status s = m_Db->Delete(leveldb::WriteOptions(), key);
	if (!s.ok()) {
		cerr << s.ToString() << endl;
		return false;
	}else{
		return true;
	}

}


bool LevelDBEngine::Range(string start, string limit)//range [start,limit)
{
	leveldb::Iterator* it = m_Db->NewIterator(leveldb::ReadOptions());
	for (it->Seek(start); it->Valid() && it->key().ToString() < limit; it->Next()){
		cout << it->key().ToString() << ": "  << it->value().ToString() << endl;
		//cout <<"iterator="<<it<<endl;
	}
	
	delete it;
	
	return true;

}


bool LevelDBEngine::RangeOpen(RangeStruct *rs)
{
	leveldb::Iterator* it = m_Db->NewIterator(leveldb::ReadOptions());
	it->Seek(rs->start);
	
	rs->iterator = it;

	return rs;
}

bool LevelDBEngine::Next(RangeStruct *rs, KeyValuePair *kv)
{
	leveldb::Iterator* it = (leveldb::Iterator*)(rs->iterator);
	//cout <<"iterator="<<it<<endl;

	if(it->Valid() && it->key().ToString() < rs->limit){
	//if(it->Valid()){
		kv->key = it->key().ToString();
		kv->value = it->value().ToString();
	}else{
		delete it;
		return false;
	}

	it->Next();

	rs->iterator = it;

	return true;
}





