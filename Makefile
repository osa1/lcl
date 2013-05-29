CPP = clang++
CPPFLAGS = -g -std=c++11 -Wall -Iinclude -I/home/omer/opt/lua-5.2.2/install/include/

CC  = clang
CCFLAGS = -g -Wall -Iinclude -I/home/omer/opt/lua-5.2.2/install/include/

LDFLAGS = -L/home/omer/opt/lua-5.2.2/install/lib/ -llua -lm

containerso:
	$(CPP) $(CPPFLAGS) -c src/containers.cpp

maino:
	$(CC) $(CCFLAGS) -c main.c

main: containerso maino
	# $^
	$(CC) containers.o main.o -o main $(LDFLAGS) -lstdc++
	#$(CPP) $(CPPFLAGS) $(LDFLAGS) containers.o main.o -o main

clean:
	-rm *.o
	-rm main
