#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include <sys/mman.h>
#include"struct_helper.c"
#include<ucontext.h>

int main() {
	printf("Reading from checkpoint file\n");
	int fd = open("mycheckpoint_temp", O_RDONLY);
	char buff[100];
	int n;
	struct memory_segment proc_maps[100];
        int i = 0;
	ucontext_t context;
	n = read(fd, &context, sizeof(ucontext_t));
        //printf("context %p", context);
        setcontext(context);
	while((n = read(fd, &proc_maps[i], sizeof(struct memory_segment)))  > 0) {
        	printf("Start address is %p\n", proc_maps[i].start);
		printf("End address is %p\n", proc_maps[i].end);
		printf("Permissions %s\n", proc_maps[i].permission);
		printf("==========================================\n");
		//printf("Data size %d\n", proc_maps[i].data_size);
		i++;
	}
	//n = read(fd, &context, sizeof(ucontext_t));
	//printf("context %p", context);
	//setcontext(&context);
}
