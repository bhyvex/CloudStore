#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stdlib.h>
#include <string.h>
#include "../FSNameSpace.h"
#include "../KVNameSpace.h"
#include <errno.h>

using namespace std;

FSNameSpace fs;

int main()
{
	int ret;
	
	
	ret = fs.setRoot(".");

	ret = fs.MkDir("fs_test", S_IRWXU);
	if(ret < 0){
		cout <<"fs.MkDir error"<<endl;
	}

	ret = fs.MkDir("fs_test/aaa", S_IRWXU);
	if(ret < 0){
		cout <<"fs.MkDir error"<<endl;
	}

	Args fd1 = fs.Open("fs_test/a.txt", O_CREAT | O_EXCL | O_WRONLY, S_IRUSR | S_IWUSR);
	if(fd1< 0){
		cout <<"fs.Open error"<<endl;
	}
	Args fd2 = fs.Open("fs_test/b.txt", O_CREAT | O_EXCL | O_WRONLY, S_IRUSR | S_IWUSR);
	if(fd2 < 0){
		cout <<"fs.Open error"<<endl;
	}
	fs.Close(&fd2);
	Args fd3 = fs.Open("fs_test/c.txt", O_CREAT | O_EXCL | O_WRONLY, S_IRUSR | S_IWUSR);
	if(fd3 < 0){
		cout <<"fs.Open error"<<endl;
	}
	fs.Close(&fd3);
	Args fd4 = fs.Open("fs_test/d.txt", O_CREAT | O_EXCL | O_WRONLY, S_IRUSR | S_IWUSR);
	if(fd4 < 0){
		cout <<"fs.Open error"<<endl;
	}
	fs.Close(&fd4);
	Args fd5 = fs.Open("fs_test/aaa/a.txt", O_CREAT | O_EXCL | O_WRONLY, S_IRUSR | S_IWUSR);
	if(fd5 < 0){
		cout <<"fs.Open error"<<endl;
	}
	fs.Close(&fd5);
	Args fd6 = fs.Open("fs_test/aaa/b.txt", O_CREAT | O_EXCL | O_WRONLY, S_IRUSR | S_IWUSR);
	if(fd6 < 0){
		cout <<"fs.Open error"<<endl;
	}
	fs.Close(&fd6);

	ret = fs.Write(&fd1, "hello", 5);
	if(ret < 0){
		cout <<"fs.Write error"<<endl;
	}

	ret = fs.Close(&fd1);
	if(ret < 0){
		cout <<"fs.Close error"<<endl;
	}

	fd1 = fs.Open("fs_test/a.txt", O_RDONLY);
	if(fd1< 0){
		cout <<"fs.Open error"<<endl;
	}

	char *buffer[1024];
	memset(buffer, 0, 1024);
	ret = fs.Read(&fd1, buffer, 6);
	if(ret < 0){
		cout <<"fs.Read error"<<endl;
	}else{
		cout <<"read "<<ret<<":"<<buffer<<endl;
	}

	cout <<"1"<<endl;

	ret = fs.Close(&fd1);
	if(ret < 0){
		cout <<"fs.Close error"<<endl;
	}

	cout <<"2"<<endl;
	
	//--./fs_test
	//-----|------->aaa
	//-----|->a.txt     |->a.txt
	//-----|->b.txt     |->b.txt
	//-----|->c.txt
	//-----|->d.txt
	Args args1;
	ret = fs.OpenDir("fs_test", &args1);
	if(ret < 0){
		cout <<"fs.OpenDir error"<<endl;
	}
	cout <<"3"<<endl;

	Args args;
	Dirent dirent;

	while(fs.ReadDirNext(&args1, &dirent)){
		string filename = "fs_test/";
		filename += dirent.filename;

		cout <<"fs_test have:"<< filename <<endl;

		if(dirent.filetype == DIR_){
			ret = fs.RmDir(filename.c_str());
			if(ret < 0){
				perror("rmdir error:");
			}
		}else{

			ret = fs.Remove(filename.c_str());
			if(ret < 0){
				perror("remove error:");
			}
		}
	}



	string filename = "fs_test";
	FileAttr attr;
	ret = fs.Stat(filename.c_str(), &attr);
	if(ret < 0){
		perror("stat error:");
	}else{
		cout <<"fs_test's size = "<< attr.m_Size <<endl;
	}

	ret = fs.Move("fs_test/aaa/a.txt", "./fs_test/a.txt");
	if(ret < 0){
		perror("rename error:");
	}

	ret = fs.Link("fs_test/a.txt", "./fs_test/a_ptr.txt");
	if(ret < 0){
		perror("link error:");
	}

	ret = fs.Link("fs_test/aaa/b.txt", "./fs_test/b_ptr.txt");
	if(ret < 0){
		perror("link error:");
	}

	ret = fs.Unlink("fs_test/a_ptr.txt");
	if(ret < 0){
		perror("unlink error:");
	}

	
	cout <<"it is done."<<endl;

	
	vector<string*> hehe;
	hehe.resize(22);
	
	string str = "fdafd";
	hehe[0] = &str;

	if(hehe[0]){
		cout <<(*(hehe[0]))<<endl;
	}
	if(hehe[20]){
		cout <<(*(hehe[20]))<<endl;
	}else{
		cout <<"not found"<<endl;
	}
	

	

	
};
