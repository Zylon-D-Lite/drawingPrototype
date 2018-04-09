project: test001 test002

test001: ./source/main.cpp
	g++ -g -std=c++14 -L/usr/local/lib/ $< -lpng -o $@

test002: ./objs/src001.o ./objs/line.o ./objs/linemanager.o
	g++ -g -std=c++14 -L/usr/local/lib/ ./objs/src001.o ./objs/line.o ./objs/linemanager.o -lpng `libpng-config --cflags` -o $@

./objs/src001.o : ./source/linemanager.hpp ./source/line.hpp ./source/src001.cpp
	g++ -g -c -std=c++14 -L/usr/local/lib/ ./source/src001.cpp -lpng `libpng-config --cflags` -o $@
./objs/line.o : ./source/line.hpp ./source/line.cpp
	g++ -g -c -std=c++14 -L/usr/local/lib/ ./source/line.cpp -lpng `libpng-config --cflags` -o $@

./objs/linemanager.o : ./source/linemanager.hpp ./source/linemanager.cpp
	g++ -g -c -std=c++14 -L/usr/local/lib/ ./source/linemanager.cpp -lpng `libpng-config --cflags` -o $@