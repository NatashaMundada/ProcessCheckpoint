#include<stdio.h>
#include<fcntl.h>
#include<assert.h>
#include<string.h>
#include<signal.h>
#include<stdlib.h>
#include<ucontext.h>
#include<sys/mman.h>
#include"struct_helper.c"

#define STRUCT_LEN  1000
void my_constructor();
int main(int, char *[]);

//ucontext_t context;

//gets the memory segment using /proc/self/maps command
int get_memory_segments(struct memory_segment proc_maps[]) {
        int proc_maps_fd = open("/proc/self/maps", O_RDONLY);
        int rc = -2, rc_temp = -2;
        int i = 0, flag = 0;
        char filename[100];
        int fd_filename = open("mycheckpoint_temp", O_WRONLY|O_APPEND|O_TRUNC|O_CREAT, S_IRUSR|S_IWUSR);
	
	ucontext_t context;
	int getcontext_output = getcontext(&context);
	if (flag == 1) {
		return;
	}
	flag = 1;	
	//int getcontext_output = getcontext(&context);
	int write_rc = write_data(fd_filename, &context, sizeof(ucontext_t));

	for(i = 0; rc != EOF; i++) {
                rc = read_proc_maps_output(proc_maps_fd, &proc_maps[i], fd_filename);
        }

        close(proc_maps_fd);
	//close(fd_filename);
        return 0;
}

//Reads the /proc/self/maps command output and store it in a struct memory_segment
int read_proc_maps_output(int fd, struct memory_segment *proc_maps, int fd_ckpt) {
        unsigned long int start;
        unsigned long int end;
        char permission[4];
	char filename[60];
        int temp_stdin = dup(0);
	dup2(fd, 0);
        int rc = scanf("%lx-%lx %4c %*s %*s %*[0-9 ]%[^\n]\n", &start, &end, permission, filename);
        assert(fseek(stdin, 0, SEEK_CUR) == 0);
        dup2(temp_stdin, 0);
        close(temp_stdin);
        if(rc == EOF || rc == 0) {
                proc_maps->start = NULL;
                proc_maps->end = NULL;
                return EOF;
        }
        
	//[vsyscall] are not stored in the checkpoint file
	proc_maps->start = (void*) start;
        proc_maps->end = (void*) end;
	proc_maps->permission[0] = permission[0];
	proc_maps->permission[1] = permission[1];
        proc_maps->permission[2] = permission[2];
	proc_maps->permission[3] = permission[3];
	//memcpy(proc_maps->permission, permission, 4);
//	printf("Permission lib %s\n", proc_maps->permission);

	if(strstr(filename,"[vsyscall]"))
	{
//		printf("Not storing [vsyscall] data in the ckeckpoint file\n");
	}
	else if(strstr(filename, "[vdso]")) {
//		printf("Not storing [vdso] data in the checkpoint file\n");
	}
	//guard pages added by compiler are not stored in the checkpoint file 
	else if(strstr(proc_maps->permission,"---p")) {
//		printf("This segment is added as a guard page, so not storing it in the checkpoint file\n");
	}
	
	else {
//		printf("storing in the file\n");
		//storing the segments in a checkpoint file
		int write_rc = write(fd_ckpt, proc_maps, sizeof(struct memory_segment));		
        	int k = write_data(fd_ckpt, proc_maps->start, proc_maps->end - proc_maps->start);
//		printf("start addr %p \n", proc_maps->start);
//		printf("end addr %p \n", proc_maps->end);
		//printf("wrote bytes %d \n", k); 

		//int write_rc = write(fd_ckpt, proc_maps->start, proc_maps->end - proc_maps->start);
		if(write_rc == -1) {
			perror("error writing");
			exit(1);
	
		}
		
	}
	return 0;
}

int write_data(int fd, void *addr, int length) {
	int rc = 0;
	while (rc < length) {
		int rc2 = write(fd, (void *) addr + rc, length - rc);
		rc += rc2;
	}
	return rc;
}

//Signal handler for the SIGUSR2
void signal_handler(int signal) {
	struct memory_segment proc_maps[1000];	
	//int fd_filename = open("mycheckpoint_temp", O_WRONLY|O_APPEND, S_IRUSR|S_IWUSR);
	
	assert(get_memory_segments(proc_maps) == 0);
	printf("Memory segments are stored in mycheckpoint_temp file\n");
} 

void __attribute__((constructor))
my_constructor() {
	signal(SIGUSR2, &signal_handler);
}
