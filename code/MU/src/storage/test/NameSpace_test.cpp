#include <iostream>
#include <string>
#include <stdlib.h>
#include <string.h>
#include "../FSNameSpace.h"
#include "../KVNameSpace.h"

using namespace std;

FSNameSpace fs;

int main()
{
	int ret;
	
	ret = fs.setRoot("./");

	ret = fs.MkDir("./fs_test", S_IRWXU);
	if(ret < 0){
		cout <<"fs.MkDir error"<<endl;
	}

	ret = fs.MkDir("./fs_test/aaa", S_IRWXU);
	if(ret < 0){
		cout <<"fs.MkDir error"<<endl;
	}

	int fd1 = fs.Open("/fs_test/a.txt", O_CREAT | O_EXCL | O_WRONLY, S_IRUSR | S_IWUSR);
	if(fd1< 0){
		cout <<"fs.Open error"<<endl;
	}
	int fd2 = fs.Open("/fs_test/b.txt", O_CREAT | O_EXCL | O_WRONLY, S_IRUSR | S_IWUSR);
	if(fd2 < 0){
		cout <<"fs.Open error"<<endl;
	}
	fs.Close(fd2);
	int fd3 = fs.Open("/fs_test/c.txt", O_CREAT | O_EXCL | O_WRONLY, S_IRUSR | S_IWUSR);
	if(fd3 < 0){
		cout <<"fs.Open error"<<endl;
	}
	fs.Close(fd3);
	int fd4 = fs.Open("/fs_test/d.txt", O_CREAT | O_EXCL | O_WRONLY, S_IRUSR | S_IWUSR);
	if(fd4 < 0){
		cout <<"fs.Open error"<<endl;
	}
	fs.Close(fd4);
	int fd5 = fs.Open("/fs_test/aaa/a.txt", O_CREAT | O_EXCL | O_WRONLY, S_IRUSR | S_IWUSR);
	if(fd5 < 0){
		cout <<"fs.Open error"<<endl;
	}
	fs.Close(fd5);
	int fd6 = fs.Open("/fs_test/aaa/b.txt", O_CREAT | O_EXCL | O_WRONLY, S_IRUSR | S_IWUSR);
	if(fd6 < 0){
		cout <<"fs.Open error"<<endl;
	}
	fs.Close(fd6);

	ret = fs.Write(fd1, "hello", 5);
	if(ret < 0){
		cout <<"fs.Write error"<<endl;
	}

	ret = fs.Close(fd1);
	if(ret < 0){
		cout <<"fs.Close error"<<endl;
	}

	fd1 = fs.Open("/fs_test/a.txt", O_RDONLY);
	if(fd1< 0){
		cout <<"fs.Open error"<<endl;
	}

	char *buffer[1024];
	memset(buffer, 0, 1024);
	ret = fs.Read(fd1, buffer, 6);
	if(ret < 0){
		cout <<"fs.Read error"<<endl;
	}
	cout <<"read:"<<buffer<<endl;

	ret = fs.Close(fd1);
	if(ret < 0){
		cout <<"fs.Close error"<<endl;
	}

	//--./fs_test
	//-----|------->aaa
	//-----|->a.txt     |->a.txt
	//-----|->b.txt     |->b.txt
	//-----|->c.txt
	//-----|->d.txt
	ret = fs.ReadDir("./fs_test");
	if(ret < 0){
		cout <<"fs.ReadDir error"<<endl;
	}

	Args args;
	do{
		args = fs.ReadDirNext();
		string filename = ((struct dirent *)(args.arg1))->d_name;
		cout <<"./fs_test have:"<< filename <<endl;
		ret = fs.RmDir(filename);
		if(ret < 0){
			cout <<"fs.RmDir error"<<endl;
		}
		ret = fs.Remove(filename);
		if(ret < 0){
			cout <<"fs.Remove error"<<endl;
		}
	}while(args.arg1);
	cout <<"it is done."<<endl;

	

	

	

	
};
