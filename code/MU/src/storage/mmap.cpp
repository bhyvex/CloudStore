#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <errno.h>

using namespace std;



int main()
{
	int fd;
	int size = sizeof(uint64_t);
	uint64_t number = 0;
	string file = "./testfile1";
	
	if ((fd = open(file.c_str(), O_RDWR| O_CREAT, S_IRWXU)) < 0){ 
		perror("open() ");
		return -1;
	}

	int ret = write(fd, (void *)&number, sizeof(number));
	ret = fsync(fd);
	if(ret < 0){ 
		perror("fsync() ");
		return -1;
	}

	void* sfp;
	if((sfp = mmap(NULL, size, PROT_READ|PROT_WRITE,MAP_SHARED, fd, 0)) == NULL){
		perror("mmap() ");
		return -1;
	}

	uint64_t *ptr = (uint64_t*)sfp;

	*ptr = 0;
	ret = fsync(fd);
	if(ret < 0){ 
		perror("fsync() ");
		return -1;
	}

	cout <<*ptr<<endl;

}
