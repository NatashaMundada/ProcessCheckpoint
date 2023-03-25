#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<sys/mman.h>
#include"struct_helper.c"
#include<ucontext.h>

void do_work() {
	int rc = -2;
	printf("Reading from checkpoint file\n");
        int fd = open("mycheckpoint_temp", O_RDONLY);
        char buff[100];
        int n;
        struct memory_segment proc_maps[100];
        int i = 0;
        ucontext_t old_registers;

	n = read_data(fd, &old_registers, sizeof(ucontext_t));

	while((n = read(fd, &proc_maps[i], sizeof(struct memory_segment)))  > 0) {
        	void *addr = mmap(proc_maps[i].start, proc_maps[i].end - proc_maps[i].start, PROT_READ|PROT_WRITE|PROT_EXEC,
                    MAP_PRIVATE | MAP_FIXED | MAP_ANONYMOUS, -1, 0);
		int k = read_data(fd, addr, proc_maps[i].end - proc_maps[i].start);

		if (addr == MAP_FAILED) {
			perror("mmap error");
		}
	        i++;
        }
	int setcontext_output = setcontext(&old_registers);
}

int read_data(int fd, void *addr, int length) {
	int rc = 0;
	while(rc < length) {
		int rc2 = read(fd, (void *) addr + rc, length - rc);
		rc += rc2;
	}
	return rc;
}

void recursive(int r) {
        if (r > 0) {
                recursive(r -1);
        } else {
                do_work();
        }
}

int main() {
	recursive(10000);
}
