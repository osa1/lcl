LUA_INSTALL = /usr/local
CPP = clang++
CC  = clang

CPPFLAGS = -g -std=c++11 -Wall -Wextra -Wpedantic -Iinclude -I$(LUA_INSTALL)/include/
CCFLAGS = -g -Wall -Wextra -Wpedantic -Iinclude -I$(LUA_INSTALL)/include/
LDFLAGS = -L$(LUA_INSTALL)/lib/ -llua -lm -ldl

OBJS = main.o containers.o
SOBJS = liblcl.so containers.so

liblcl.so: containers.so main.c
	$(CC) $(CCFLAGS) -shared -rdynamic -fPIC main.c -o liblcl.so

main: containers.o main.o
	$(CC) $^ -o main $(LDFLAGS) -lstdc++

containers.o: src/containers.cpp
	$(CPP) $(CPPFLAGS) -c $<

main.o: main.c
	$(CC) $(CCFLAGS) -c $<

containers.so: src/containers.cpp
	$(CPP) $(CPPFLAGS) -shared -rdynamic -fPIC  $< -o containers.so

clean:
	-rm $(OBJS)
	-rm $(SOBJS)
	-rm main
