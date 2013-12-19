/*
 * @file main.cpp
 * @brief main .... what can I say about it ....
 *
 * @version 1.0
 * @date Mon Jul  2 20:53:36 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#include <string>
#include <iostream>

#include <signal.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>

#include "MUConfiguration.h"
#include "RunControl.h"


#include "storage/ChannelMappingStrategy.h"
#include "storage/Channel.h"
#include "storage/ChannelManager.h"
#include "storage/KVNameSpace.h"
#include "storage/FSNameSpace.h"
#include "storage/HashMappingStrategy.h"
#include "storage/Key.h"
#include "storage/KeyValuePair.h"
#include "storage/RangeStruct.h"
#include "storage/SplitPathStrategy.h"
#include "data/FileMeta.h"
#include "sys/ThreadPool.h"
#include "sys/DoubleQueue.h"

DoubleQueue<uint64_t> queue;
uint64_t num;


static void sighandler(int sig_no)
{
    exit(0);
}

void usage(const std::string &exeName)
{
    fprintf(stderr, "Usage: %s [-c configFilePath]\n", exeName.c_str());
}


string IntToString(int num)
{
	int ss;
	char temp[64];
	memset(temp, 0, 64);
	sprintf(temp, "%d", num);
	string s(temp);

	return s;
}

class EchoItem : public ThreadPoolWorkItem {
    public:
        int process() {
            int a;
            a++;
            a++;
        }
};

class HeheThread : public Thread
{
public:
	void run()
	{
		while(1){
			while(queue.size() != 0){
				queue.pop();
			}
		}
	}
}



int main(int argc, char *argv[])
{
//-----------------------------------MU------------------------------------------------------------
/*

    signal(SIGINT, sighandler);
    signal(SIGUSR1, sighandler);

    if (1 == argc) {
        usage(argv[0]);
        exit(1);
    }

    int rt = 0;

    std::string configFileName;

    // parse arguments
    int opt = 0;

    while (-1 != (opt = getopt(argc, argv, "c:"))) {
        switch (opt) {
        case 'c':
            configFileName = optarg;
            break;

        default:
            usage(argv[0]);
            exit(1);
            break;
        }
    }

    rt = MUConfiguration::getInstance()->configWithXML(configFileName);

    if (-1 == rt) {
        fprintf(stderr, "Load configuration from file %s failed. Program will exit.",
                  configFileName.c_str());
        exit(1);
    }

    RunControl::getInstance()->initialize();

    RunControl::getInstance()->run();

    return 0;
*/

//------------------------------------KVNameSpace test-----------------------------------------------------------

/*

	//test Channel 
	MUConfiguration::getInstance()->m_ChannelNum = 4;
	MUConfiguration::getInstance()->m_MainChannelID = 3;
	ChannelManager::getInstance()->init();
	ChannelManager::getInstance()->createChannel(0, "/home/data/Channel0");

	Channel* pDataChannel = ChannelManager::getInstance()->findChannel(0);
	NameSpace *DataNS = pDataChannel->m_DataNS;





	string path_root = "bucket0/user1";
	string path_home = "bucket0/user1/home";
	string path_home_a = "bucket0/user1/home/a";
	string path_home_a_b = "bucket0/user1/home/a/b";
	string file_home_a_b_atxt = "bucket0/user1/home/a/b/a.txt";
	string file_b_txt = "bucket0/user1/b.txt";


	int ret = DataNS->MkDir(path_root.c_str(), 0);
	if(ret < 0){
		cout <<"MkDir() error"<<endl;
		exit(0);
	}
	ret = DataNS->MkDir(path_home.c_str(), 0);
	if(ret < 0){
		cout <<"MkDir() error"<<endl;
		exit(0);
	}
	ret = DataNS->MkDir(path_home_a.c_str(), 0);
	if(ret < 0){
		cout <<"MkDir() error"<<endl;
		exit(0);
	}
	ret = DataNS->MkDir(path_home_a_b.c_str(), 0);
	if(ret < 0){
		cout <<"MkDir() error"<<endl;
		exit(0);
	}


	FileAttr fa;

	ret = DataNS->Stat(path_root.c_str(), &fa);
	if(ret < 0){
		cout <<"Stat() error"<<endl;
		exit(0);
	}
	ret = DataNS->Stat(path_home.c_str(), &fa);
	if(ret < 0){
		cout <<"Stat() error"<<endl;
		exit(0);
	}
	ret = DataNS->Stat(path_home_a.c_str(), &fa);
	if(ret < 0){
		cout <<"Stat() error"<<endl;
		exit(0);
	}
	ret = DataNS->Stat(path_home_a_b.c_str(), &fa);
	if(ret < 0){
		cout <<"Stat() error"<<endl;
		exit(0);
	}
	

	
	Args file_home_a_b_atxt_fd = DataNS->Open(file_home_a_b_atxt.c_str(), 0);
	if(file_home_a_b_atxt_fd.valid == false){
		cout <<"Open() error"<<endl;
		exit(0);
	}

	Args file_b_txt_fd = DataNS->Open(file_b_txt.c_str(), 0);
	if(file_b_txt_fd.valid == false){
		cout <<"Open() error"<<endl;
		exit(0);
	}

	fa.m_FID = 0;
	fa.m_Size = 0;
	fa.m_Type = 1;
	fa.m_Version = 1;
	string buffer_str((char*)(&fa), sizeof(fa));
	ssize_t size = DataNS->writen(&file_home_a_b_atxt_fd, buffer_str.data(), buffer_str.size());
	if(size < 0){
		cout <<"writen() error"<<endl;
		exit(0);
	}
	size = DataNS->writen(&file_b_txt_fd, buffer_str.data(), buffer_str.size());
	if(size < 0){
		cout <<"writen() error"<<endl;
		exit(0);
	}

	char buffer[1024];
	memset(buffer, 0, 1024);
	size = DataNS->readn(&file_home_a_b_atxt_fd, buffer, buffer_str.size());
	if(size < 0){
		cout <<"readn() error"<<endl;
		exit(0);
	}
	cout <<"read buffer:="<<buffer<<endl;
	
	Args dir;
	ret = DataNS->OpenDir(path_root.c_str(), &dir);
	if(ret < 0){
		cout <<"OpenDir() error"<<endl;
		exit(0);
	}

	Dirent dirent;
	while(DataNS->ReadDirNext(&dir, &dirent)){
		cout << "filename:"<<dirent.filename<<"  type:"<<dirent.filetype<<endl;
	}
	
	*/


	/*

	int DataNS->RmDir(const char *pathname);





	int DataNS->Remove(const char *pathname);



	int DataNS->Move(const char *oldpath, const char *newpath);
	int DataNS->Link(const char *oldpath, const char *newpath);
	int DataNS->Unlink(const char *pathname);
	int DataNS->RmdirRecursive(const char *pathname);
	*/



	//------------------------------------------leveldb test--------------------------------------------------------------

/*
	LevelDBEngine db(".");

	bool ret;

	for(int i = 0; i < 1; i++){
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
   */

   /*
	string key = "1/0/0";
	FileAttr fa;
	fa.m_FID = 312;
	fa.m_Version = 77;
	char buffer[sizeof(fa)];
	memset(buffer, 0, sizeof(fa));
	memcpy(buffer, &fa, sizeof(fa));
	string value(buffer, sizeof(fa));
	ret = db.Put(key, value);
	if(!ret){
	    cout <<"db.Put() error"<<endl;                                                                  
	}   
   
	string value1;
	ret = db.Get("1/0/0", value1);
	if(ret == false){
		cout <<"get error."<<endl;
	}else{
		FileAttr fa1;
		fa1 = *(FileAttr*)(value1.data());
		cout <<"fa1.m_FID="<<fa1.m_FID<<endl;
	}

	//db.Range("1/0/", "1/1/");
	cout <<"--------"<<endl;
	*/

	//-----------------------------Theadpool-----------------
	/*
	ThreadPool *pool = new ThreadPool();
    pool->start();

    TimeCounter tc;
    tc.begin();
    for (int i = 0; i < 1000000; ++i) {
        pool->postRequest(new EchoItem());
    }
    tc.end();
    cout <<tc.diff()<<endl;

    sleep(5);
    */

    //-----------------------DoubleQueue---------------------------------

	HeheThread t1;
	t1.start();
    
    
    for(uint64_t i = 0; i < 1000; i++){
    	queue.push(i);
    	if(i % 10000 ==0){
	    	cout <<"num"<<endl;
	    }
    }
    
    while(){
    }

    
    
	
	return 0;
}

