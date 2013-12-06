#include "SplitPathStrategy.h"
#include "frame/MUMacros.h"
#include "Key.h"
#include "Value.h"
#include "LevelDBEngine.h"

#define ROOT_FILEID "0"

SplitPathStrategy::SplitPathStrategy()
{
	m_StoreEngine = new LevelDBEngine();
}

SplitPathStrategy::~SplitPathStrategy()
{
}

/*
  * @pathname such as "bucket1/user1/a/a.txt"  "bucket1/user1/a.txt"
  *
  */
int SplitPathStrategy::PutEntry(string pathname, const char* buf, int n)
{
	bool ret = false;
	size_t pos;

	//get bucket string
	pos = pathname.find_first_of(PATH_SEPARATOR_STRING);//BUCKET_NAME_PREFIX
	string bucket = pathname.substr(0, pos);
	pathname = pathname.substr(pos+1);//"user1/a/a.txt"

	//get user string
	pos = pathname.find_first_of(PATH_SEPARATOR_STRING);
	string user = pathname.substr(0, pos);//"user1"
	string userID = user.substr(sizeof(USER_NAME_PREFIX)-1);
	pathname = pathname.substr(pos+1);//"a/a.txt" or "a.txt"

	//find the p_fid and filename
	string ParentDir;
	string FileName;//maybe a dir
	pos = pathname.rfind(PATH_SEPARATOR_STRING);
	if (pos == string::npos){//is "a.txt"
		ParentDir = "";
		FileName = pathname;
	}else{//is "a/a.txt"
		ParentDir = pathname.substr(0, pos);
		FileName = pathname.substr(pos+1);
	}

	string p_fid;
	ret = FindEntryID(ParentDir, userID, p_fid);
	if(ret == false){
		return -1;
	}
	

	/* make the key */
	MUKeyInfo keyinfo;
	keyinfo.userID = userID;
	keyinfo.PID = p_fid;
	keyinfo.FileName = FileName;
	string key = Key::serialize(&keyinfo);

	/* make the value */
	string value(buf, n);

	/* insert to db */
	ret = m_StoreEngine->Put(key, value);
	if(ret == false){
	}

	return 0;
}

int SplitPathStrategy::GetEntry(string pathname, char *buf, int *n)
{
	bool ret = false;
	size_t pos;

	//get bucket string
	pos = pathname.find_first_of(PATH_SEPARATOR_STRING);//BUCKET_NAME_PREFIX
	string bucket = pathname.substr(0, pos);
	pathname = pathname.substr(pos+1);//"user1/a/a.txt"

	//get user string
	pos = pathname.find_first_of(PATH_SEPARATOR_STRING);
	string user = pathname.substr(0, pos);//"user1"
	string userID = user.substr(sizeof(USER_NAME_PREFIX)-1);
	pathname = pathname.substr(pos+1);//"a/a.txt" or "a.txt"

	//find the p_fid and filename
	string ParentDir;
	string FileName;//maybe a dir
	pos = pathname.rfind(PATH_SEPARATOR_STRING);
	if (pos == string::npos){//is "a.txt"
		ParentDir = "";
		FileName = pathname;
	}else{//is "a/a.txt"
		ParentDir = pathname.substr(0, pos);
		FileName = pathname.substr(pos+1);
	}

	string p_fid;
	ret = FindEntryID(ParentDir, userID, p_fid);
	if(ret == false){
		return -1;
	}
	

	/* make the key */
	MUKeyInfo keyinfo;
	keyinfo.userID = userID;
	keyinfo.PID = p_fid;
	keyinfo.FileName = FileName;
	string key = Key::serialize(&keyinfo);

	/* get from db */
	string value;
	ret = m_StoreEngine->Get(key, value);
	if(ret == false){
	}

	memcpy(buf, value.c_str(), value.size());
	*n = value.size();

	return 0;
}

int SplitPathStrategy::DeleteEntry(string pathname)
{
	bool ret = false;
	size_t pos;

	//get bucket string
	pos = pathname.find_first_of(PATH_SEPARATOR_STRING);//BUCKET_NAME_PREFIX
	string bucket = pathname.substr(0, pos);
	pathname = pathname.substr(pos+1);//"user1/a/a.txt"

	//get user string
	pos = pathname.find_first_of(PATH_SEPARATOR_STRING);
	string user = pathname.substr(0, pos);//"user1"
	string userID = user.substr(sizeof(USER_NAME_PREFIX)-1);
	pathname = pathname.substr(pos+1);//"a/a.txt" or "a.txt"

	//find the p_fid and filename
	string ParentDir;
	string FileName;//maybe a dir
	pos = pathname.rfind(PATH_SEPARATOR_STRING);
	if (pos == string::npos){//is "a.txt"
		ParentDir = "";
		FileName = pathname;
	}else{//is "a/a.txt"
		ParentDir = pathname.substr(0, pos);
		FileName = pathname.substr(pos+1);
	}

	string p_fid;
	ret = FindEntryID(ParentDir, userID, p_fid);
	if(ret == false){
		return -1;
	}
	

	/* make the key */
	MUKeyInfo keyinfo;
	keyinfo.userID = userID;
	keyinfo.PID = p_fid;
	keyinfo.FileName = FileName;
	string key = Key::serialize(&keyinfo);

	/* delete from db */
	ret = m_StoreEngine->Delete(key);
	if(ret == false){
	}

	return 0;
}


/* 
  * @pathname = "home/lpc"
  */
bool SplitPathStrategy::FindEntryID(string pathname, string userID, string &fid)
{
	/* init */
	size_t pos;
	string dir;
	string postfix;
	string pid = ROOT_FILEID;
	string key;
	string value;
	int ret;
	MUKeyInfo keyinfo;
	MUValueInfo valueinfo;
	
	if(pathname == string("")){//null string
		fid = pid;
	}

	/* traverse the dir from pathname */
	postfix = pathname;
	while((pos = postfix.find_first_of(PATH_SEPARATOR_STRING)) != string::npos){
		dir = postfix.substr(0, pos);
		postfix = postfix.substr(pos+1);
		

		keyinfo.userID = userID;
		keyinfo.PID = pid;
		keyinfo.FileName = dir;
		key = Key::serialize(&keyinfo);
		ret = m_StoreEngine->Get(key, value);
		if(ret == false){
			return false;
		}

		valueinfo = Value::deserialize(value);
		pid = valueinfo.fid;

	}

	//traverse to the end
	keyinfo.userID = userID;
	keyinfo.PID = pid;
	keyinfo.FileName = postfix;
	key = Key::serialize(&keyinfo);
	ret = m_StoreEngine->Get(key, value);
	if(ret == false){
		return false;
	}
	valueinfo = Value::deserialize(value);
	pid = valueinfo.fid;
	
	if(pid < string(ROOT_FILEID)){
		return false;
	}else{
		return true;
	}
	
	
}