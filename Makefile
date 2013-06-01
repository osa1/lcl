LUA_INSTALL = /home/omer/opt/lua-5.2.2/install

CPP = clang++
CPPFLAGS = -g -std=c++11 -Wall -Wextra -Wpedantic -Iinclude -I$(LUA_INSTALL)/include/

CC  = clang
CCFLAGS = -g -Wall -Wextra -Wpedantic -Iinclude -I$(LUA_INSTALL)/include/

LDFLAGS = -L$(LUA_INSTALL)/lib/ -llua -lm -ldl

OBJS = main.o containers.o
SOBJS = main.so containers.so

main: containers.o main.o
	$(CC) $^ -o main $(LDFLAGS) -lstdc++

containers.o: src/containers.cpp
	$(CPP) $(CPPFLAGS) -c $<

main.o: main.c
	$(CC) $(CCFLAGS) -c $<

containers.so: src/containers.cpp
	$(CPP) $(CPPFLAGS) -shared -rdynamic -fPIC  $< -o containers.so

main.so: containers.so main.c
	$(CC) $(CCFLAGS) -shared -rdynamic -fPIC main.c -o main.so

clean:
	-rm $(OBJS)
	-rm $(SOBJS)
	-rm main
