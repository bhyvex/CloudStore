#include <iostream>
#include <string.h>

#include <sstream>

using namespace std;

#define PATH_SEPARATOR_STRING "/"
#define USER_NAME_PREFIX "user"
#define KEY_SEPARATOR "/"


template <typename D, typename S>
D conv(const S &s) {
    std::stringstream ss;
    ss << s;
    D d;
    ss >> d;
    return d;
}



int main(int argc, char **argv)
{
	string pathname = "bucket1/user1/fdasf/asdfasdf/a.txt";
	cout <<"now pathname="<<pathname<<endl;

	size_t pos;

	//get bucket string
	pos = pathname.find_first_of(PATH_SEPARATOR_STRING);//BUCKET_NAME_PREFIX
	string bucket = pathname.substr(0, pos);
	cout <<"bucket="<<bucket<<endl;
	pathname = pathname.substr(pos+1);//"user1/a/a.txt"

	//get user string
	cout <<"now pathname="<<pathname<<endl;
	pos = pathname.find_first_of(PATH_SEPARATOR_STRING);
	string user = pathname.substr(0, pos);//"user1"
	string userID = user.substr(sizeof(USER_NAME_PREFIX)-1);
	cout <<"user="<<user<<endl;
	cout <<"userID="<<userID<<endl;
	pathname = pathname.substr(pos+1);//"a/a.txt" or "a.txt"

	//find the p_fid
	cout <<"now pathname="<<pathname<<endl;
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
	cout <<"ParentDir="<<ParentDir<<endl;
	cout <<"FileName="<<FileName<<endl;

	string key = userID + "3" + FileName;
	cout <<"key="<<key<<endl;



	cout <<"--------------------------"<<endl;


	key = "10/3/a.txt";
	cout <<"key="<<key<<endl;

	pos = key.find(KEY_SEPARATOR);
	string userID2 = key.substr(0, pos);
	key = key.substr(pos+1);
	cout <<"userID="<<userID2<<endl;

	pos = key.find(KEY_SEPARATOR);
	string PID = key.substr(0, pos);
	key = key.substr(pos+1);
	cout <<"PID="<<PID<<endl;

	string FileName2 = key;
	cout <<"FileName="<<FileName2<<endl;

	
	cout <<"--------------------------"<<endl;

	string str1 = "0";
	string str2 = "0";
	if(str1 >= str2){
		cout <<"str1 >= str2"<<endl;
	}else if(str1 < str2){
		cout <<"str1 < str2"<<endl;
	}



	cout <<"--------------------------"<<endl;

	pathname = "home/lpc/a/abx";
	cout <<"now pathname="<<pathname<<endl;
	string dir;
	string postfix;
	string pid = "0";
	string value;
	int ret;
	string fid;
	
	if(pathname == string("")){//null string
		fid = pid;
	}

	/* traverse the dir from pathname */
	postfix = pathname;
	while((pos = postfix.find_first_of(PATH_SEPARATOR_STRING)) != string::npos){
		dir = postfix.substr(0, pos);
		postfix = postfix.substr(pos+1);
		
		string key = userID + PATH_SEPARATOR_STRING + pid +PATH_SEPARATOR_STRING +dir;
		cout <<"key="<<key<<endl;

		//get value by key
		//update pid by value

	}

	dir = postfix;
	key = userID + PATH_SEPARATOR_STRING + pid +PATH_SEPARATOR_STRING +dir;
	cout <<"key="<<key<<endl;

	cout <<"--------------------------"<<endl;

	int bucketId = 15;
	string buckeyId_str = conv<std::string, uint64_t>(bucketId);
	cout <<"old bucketid = "<<buckeyId_str<<endl;

	int newId = conv<uint64_t, std::string>(buckeyId_str);
	newId++;
	string newId_str = conv<std::string, uint64_t>(newId);
	cout <<"new bucketid = "<<newId_str<<endl;
	

}

