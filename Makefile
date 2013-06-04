LUA_INSTALL = /usr/local
CPP = clang++
CC  = clang

CPPFLAGS = -g -std=c++11 -Wall -Wextra -Wpedantic -Iinclude -I$(LUA_INSTALL)/include/
CCFLAGS = -g -Wall -Wextra -Wpedantic -Iinclude -I$(LUA_INSTALL)/include/
LDFLAGS = -L$(LUA_INSTALL)/lib/ -llua -lm -ldl

OBJS = main.o containers.o
SOBJS = liblcl.so

liblcl.so: containers.o main.o
	$(CC) $(CCFLAGS) -shared -rdynamic -fPIC -lstdc++ $^ -o $@

main: containers.o main.o
	$(CC) $^ -o main $(LDFLAGS) -lstdc++

containers.o: src/containers.cpp
	$(CPP) $(CPPFLAGS) -fPIC -c $<

main.o: main.c
	$(CC) $(CCFLAGS) -fPIC -c $<

clean:
	-rm $(OBJS)
	-rm $(SOBJS)
	-rm main
