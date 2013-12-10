#include <iostream>
#include <string.h>
#include <stdlib.h>

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

struct ddd
{
	int a;
	int b;
	char c;
};
int comp(string a, string b)
{
	string str_a = a;
	string str_b = b;
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
	cout <<"userid is equal"<<endl;
	
	if(pid_a_num < pid_b_num) return -1;
	if(pid_a_num > pid_b_num) return +1;
	cout <<"pid is equal"<<endl;
	
	if(filename_a < filename_b) return -1;
	if(filename_a > filename_b) return +1;
	cout <<"filename is equal"<<endl;
	
	return 0;

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
	
	cout <<"--------------------------"<<endl;

	cout <<"comp(1/0/, 1/0/0)="<< comp("1/0/", "1/0/0") <<endl;
	cout <<"comp(1/1/, 1/0/0)="<< comp("1/1/", "1/0/0") <<endl;

	cout <<"--------------------------"<<endl;

	ddd d;
	d.a = 123;
	d.b = 22;
	d.c = 1;
	char buffer[sizeof(ddd)];
	memset(buffer, 0, sizeof(ddd));
	memcpy(buffer, &d, sizeof(ddd));

	string str(buffer, sizeof(ddd));
	cout <<"size="<<str.size()<<endl;
	

}

