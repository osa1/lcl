LUA_INSTALL = /home/omer/opt/lua-5.2.2/install

CPP = clang++
CPPFLAGS = -g -std=c++11 -Wall -Iinclude -I$(LUA_INSTALL)/include/

CC  = clang
CCFLAGS = -g -Wall -Iinclude -I$(LUA_INSTALL)/include/

LDFLAGS = -L$(LUA_INSTALL)/lib/ -llua -lm -ldl

main: containers.o main.o
	$(CC) $^ -o main $(LDFLAGS) -lstdc++

containers.o: src/containers.cpp
	$(CPP) $(CPPFLAGS) -c $<

main.o: main.c
	$(CC) $(CCFLAGS) -c $<

clean:
	-rm *.o
	-rm main
