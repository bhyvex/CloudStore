#include "SplitPathStrategy.h"
#include "frame/MUMacros.h"
#include "Key.h"
#include "LevelDBEngine.h"

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

	string p_fid = FindEntryID(ParentDir);
	

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

	string p_fid = FindEntryID(ParentDir);
	

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

	string p_fid = FindEntryID(ParentDir);
	

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

string SplitPathStrategy::FindEntryID(string pathname)
{
	/* init */

	/* traverse the dir from pathname */

	/* traverse end, then find the final dir */

	return "";
	
}