#include "SplitPathStrategy.h"
#include "frame/MUMacros.h"
#include "Key.h"
#include "Value.h"
#include "LevelDBEngine.h"
#include "util/util.h"
#include "data/FileMeta.h"

#define ROOT_FILEID "0"

SplitPathStrategy::SplitPathStrategy(string path):
	BuildStrategy(path)
{
	m_StoreEngine = new LevelDBEngine(m_Root);
}

SplitPathStrategy::~SplitPathStrategy()
{
}

/*
  * @pathname such as "bucket1/user1/a/a.txt"  "bucket1/user1/a.txt" "bucket1/user1"
  *
  */
int SplitPathStrategy::PutEntry(string pathname, const char* buf, int n)
{
	cout <<"SplitPathStrategy::PutEntry(string pathname, const char* buf, int n)"<<endl;
	cout <<"	pathname="<<pathname<<endl;
	bool ret = false;
	bool isroot = false;
	size_t pos;
	string user;
	string userID;

	//get bucket string
	pos = pathname.find_first_of(PATH_SEPARATOR_STRING);//BUCKET_NAME_PREFIX
	string bucket = pathname.substr(0, pos);
	pathname = pathname.substr(pos+1);//"user1/a/a.txt"

	//get user string
	pos = pathname.find_first_of(PATH_SEPARATOR_STRING);
	if(pos == string::npos){
		isroot = true;
	}
	user = pathname.substr(0, pos);//"user1"
	userID = user.substr(sizeof(USER_NAME_PREFIX)-1);
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
	cout <<"	ret = FindEntryID(ParentDir, userID, p_fid)="<<ret<<endl;
	if(ret == false){
		return -1;
	}
	

	/* make the key */
	MUKeyInfo keyinfo;
	keyinfo.userID = userID;
	keyinfo.PID = p_fid;
	keyinfo.FileName = FileName;
	if(isroot){
		keyinfo.FileName = ".";
	}
	string key = Key::serialize(&keyinfo);
	cout <<"PutEntry(string pathname, const char* buf, int n) key="<<key<<endl;

	/* make the value */
	string value(buf, n);

	/* insert to db */
	ret = m_StoreEngine->Put(key, value);
	cout <<"	ret = m_StoreEngine->Put(key, value)="<<ret<<endl;
	if(ret == false){
		return -1;
	}

	return 0;
}

//pathname = "bucket10/user2"
//pathname = "bucket10/user2/hehe"
//pathname = "bucket10/user2/hehe/a.txt"
int SplitPathStrategy::GetEntry(string pathname, char *buf, int *n)
{
	cout <<"SplitPathStrategy::GetEntry(string pathname, char *buf, int *n) 0"<<endl;
	cout <<"	pathname="<<pathname<<endl;
	
	bool ret = false;
	size_t pos;
	bool isroot = false;

	//get bucket string
	pos = pathname.find_first_of(PATH_SEPARATOR_STRING);//BUCKET_NAME_PREFIX
	string bucket = pathname.substr(0, pos);
	pathname = pathname.substr(pos+1);//"user1/a/a.txt"

	//get user string
	pos = pathname.find_first_of(PATH_SEPARATOR_STRING);
	if(pos == string::npos){
		isroot = true;
	}
	string user = pathname.substr(0, pos);//"user1"
	string userID = user.substr(sizeof(USER_NAME_PREFIX)-1);
	pathname = pathname.substr(pos+1);//"a/a.txt" or "a.txt" or ""

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
	cout <<"SplitPathStrategy::GetEntry(string pathname, char *buf, int *n) 1="<<ret<<endl;
	cout <<"	ParentDir="<<ParentDir<<endl;
	if(ret == false){
		*n = -1;
		return -1;
	}
	

	/* make the key */
	MUKeyInfo keyinfo;
	keyinfo.userID = userID;
	keyinfo.PID = p_fid;
	keyinfo.FileName = FileName;
	if(isroot){
		keyinfo.FileName = ".";
	}
	string key = Key::serialize(&keyinfo);

	//drop the last "/" if key has
	if(key[key.size()-1] == '/'){
		key = key.substr(0, key.size() - 1);
	}

	/* get from db */
	string value;
	ret = m_StoreEngine->Get(key, value);
	cout <<"SplitPathStrategy::GetEntry(string pathname="<<key<<", char *buf, int *n) 2="<<ret<<"value.size()="<<value.size()<<endl;
	if(ret == false){
		*n = -1;
		return -1;
	}

	memcpy(buf, value.c_str(), value.size());
	*n = value.size();

	return 0;
}

int SplitPathStrategy::DeleteEntry(string pathname)
{
	bool ret = false;
	size_t pos;
	bool isroot = false;

	//get bucket string
	pos = pathname.find_first_of(PATH_SEPARATOR_STRING);//BUCKET_NAME_PREFIX
	string bucket = pathname.substr(0, pos);
	pathname = pathname.substr(pos+1);//"user1/a/a.txt"

	//get user string
	pos = pathname.find_first_of(PATH_SEPARATOR_STRING);

	if(pos == string::npos){
		isroot = true;
	}
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
	if(isroot){
		keyinfo.FileName = ".";
	}
	string key = Key::serialize(&keyinfo);

	/* delete from db */
	ret = m_StoreEngine->Delete(key);
	if(ret == false){
		return -1;
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
	int pid_num = 0;
	string pid = ROOT_FILEID;
	string key;
	string value;
	int ret;
	MUKeyInfo keyinfo;
	MUValueInfo valueinfo;
	
	if(pathname == string("")){//null string
		fid = pid;
		return true;
	}

	//cout <<"FindEntryID(string pathname, string userID, string &fid) 1 pathname="<<pathname<<endl;
	/* traverse the dir from pathname */
	postfix = pathname;
	while((pos = postfix.find_first_of(PATH_SEPARATOR_STRING)) != string::npos){
		//cout <<"FindEntryID(string pathname, string userID, string &fid) 2"<<endl;
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

		FileAttr fa;
		fa = *(FileAttr *)(value.data());
		pid_num = fa.m_FID;
		pid = util::conv::conv<std::string, uint64_t>(pid_num);

	}

	//traverse to the end
	keyinfo.userID = userID;
	keyinfo.PID = pid;
	keyinfo.FileName = postfix;
	key = Key::serialize(&keyinfo);
	//cout <<"FindEntryID(string pathname, string userID, string &fid) last key="<<key<<endl;
	ret = m_StoreEngine->Get(key, value);
	if(ret == false){
		return false;
	}
	//cout <<"1"<<endl;
	
	FileAttr fa;
	fa = *(FileAttr *)(value.data());
	pid_num = fa.m_FID;
	pid = util::conv::conv<std::string, uint64_t>(pid_num);
	fid = pid;
	
	//cout <<"find pid="<<pid<<endl;
	if(pid < string(ROOT_FILEID)){
		return false;
	}else{
		return true;
	}
	
	//cout <<"1"<<endl;
	
	
}


//pathname = "bucket1/user1/a"
bool SplitPathStrategy::DirOpen(string pathname, RangeStruct *rs)
{
	size_t pos;
	bool isroot = false;
	rs->start = "";
	rs->limit = "";
	rs->iterator = NULL;

	//get bucket string
	pos = pathname.find_first_of(PATH_SEPARATOR_STRING);//BUCKET_NAME_PREFIX
	string bucket = pathname.substr(0, pos);
	pathname = pathname.substr(pos+1);//"user1/a/a.txt"

	//get user string
	pos = pathname.find_first_of(PATH_SEPARATOR_STRING);
	if(pos == string::npos){
		isroot = true;
	}
	string user = pathname.substr(0, pos);//"user1"
	string userID = user.substr(sizeof(USER_NAME_PREFIX)-1);
	pathname = pathname.substr(pos+1);//"a/a.txt" or "a.txt"
	if(isroot){
		pathname = ".";
	}

	//find the fid
	string fid;
	bool ret = FindEntryID(pathname, userID, fid);
	if(ret == false){
		return false;
	}
	

	/* make the key */
	MUKeyInfo keyinfo;
	string start = userID + KEY_SEPARATOR + fid + KEY_SEPARATOR;
	uint64_t end = util::conv::conv<uint64_t, std::string>(fid);
	end++;
	string limit = userID + KEY_SEPARATOR + util::conv::conv< std::string, uint64_t>(end) + KEY_SEPARATOR;
	//cout <<"start = "<<start<<endl;
	//cout <<"limit = "<<limit<<endl;

	rs->start = start;
	rs->limit = limit;

	ret = m_StoreEngine->RangeOpen(rs);
	if(ret == false){
		cout <<"RangeOpen error."<<endl;
		return false;
	}
	return true;
}

bool SplitPathStrategy::Next(RangeStruct *rs, KeyValuePair *kv)
{
	return m_StoreEngine->Next(rs, kv);
}