#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>


int main()
{
	int fd = open("treedb.kct", O_RDONLY);

	int ret;
	char *ptr;
	while(1){
		ptr = malloc(sizeof(char)*1024*1024);//1M
		if(ptr == NULL){
			perror("malloc:");
		}
		ret = read(fd, ptr, sizeof(char)*1024*1024);
		if(ret < 0){
			perror("read:");
		}
	}
	return 0;
}
