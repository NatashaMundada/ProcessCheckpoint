#include<stdio.h>
#include<unistd.h>

/* This program sets the environment variable LD_PRELOAD 
 * and executes the given executable
 */

int main(int argc, char *argv[]) {
	setenv("LD_PRELOAD", "./libckpt.so", 1);
	execvp(argv[0], &argv[1]);
	return 0;
} 
