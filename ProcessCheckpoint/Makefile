# You may decide to replace 'gcc' by 'clang' if you have 'clang on your
# local machine.  It has better error reporting.

CC=gcc
CFLAGS=-g3 -O0

all: ckpt libckpt.so restart

#========================

ckpt.o: ckpt.c 
	gcc -g3 -O0 -fPIC -c ckpt.c

libckpt.o: 
	gcc -g3 -O0 -fPIC -c libckpt.c

restart:
	gcc -static \
        -Wl,-Ttext-segment=5000000 -Wl,-Tdata=5100000 -Wl,-Tbss=5200000 \
        -g3 -o restart restart.c

ckpt: ckpt.o
	gcc -g3 -O0 -fPIC -o ckpt ckpt.o


libckpt.so: libckpt.c 
	gcc -g3 -O0 -shared -fPIC -o libckpt.so libckpt.c


clean:
	rm -f ckpt ckpt.o restart libckpt.so libckpt libckpt.o

check: ckpt restart helloWorld
	rm -f mycheckpoint_temp
	./ckpt ./helloWorld  &
	sleep 2.5
	echo checkpointing...
	pgrep -n helloWorld | xargs kill -12
	sleep 2.5
	echo Restarting...
	pkill -9 helloWorld 
	./restart 
