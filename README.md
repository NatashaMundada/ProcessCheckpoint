# Process Checkpoint
 Process Checkpoint/ Restart in Linux


A process checkpoint or restart allows users to stop a process, save the state of the process on disk and restart the process on the same/ new machine. 

Use the provided Makefile to run the program. steps to run the program:- 

1. make clean 
2. make all 
3. make check 

For this application, we are using a simple hello world program that prints "hello world {iteration_count}". You can replace this program with a program whose state you want to save to a file. 

The state of a program is being saved to mycheckpoint_temp file. 
