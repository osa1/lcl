LUA_INSTALL = /usr/local
CPP = clang++
CC  = clang

CPPFLAGS = -g -std=c++11 -Wall -Wextra -Wpedantic -O3 -Iinclude -I$(LUA_INSTALL)/include/
CCFLAGS = -g -Wall -Wextra -Wpedantic -O3 -Iinclude -I$(LUA_INSTALL)/include/
LDFLAGS = -L$(LUA_INSTALL)/lib/ -llua -lm -ldl

OBJS = main.o containers.o
SOBJS = liblcl.so

liblcl.so: containers.o main.o
	$(CC) $(CCFLAGS) -shared -fpic -lstdc++ $^ -o $@

main: containers.o main.o
	$(CC) $^ -o main $(LDFLAGS) -lstdc++

containers.o: src/containers.cpp
	$(CPP) $(CPPFLAGS) -fpic -c $<

main.o: main.c
	$(CC) $(CCFLAGS) -fpic -c $<

clean:
	-rm $(OBJS)
	-rm $(SOBJS)
	-rm main
