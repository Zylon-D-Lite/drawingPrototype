CPPFLAGS=g++
project: ./examples/test001 ./examples/test002 ./examples/test003

./examples/test001: ./source/main.cpp
	$(CPPFLAGS) -g -std=c++14 -L/usr/local/lib/ $< -lpng -o $@

./examples/test002: ./objs/src001.o ./objs/line.o ./objs/linemanager.o ./objs/edgemaker.o
	$(CPPFLAGS) -g -std=c++14 -L/usr/local/lib/ ./objs/src001.o ./objs/line.o ./objs/linemanager.o -lpng `libpng-config --cflags` -o $@

./examples/test003: ./objs/src002.o ./objs/edgemaker.o
	$(CPPFLAGS) -g -std=c++14 -L/usr/local/lib/ ./objs/src002.o ./objs/edgemaker.o -lpng `libpng-config --cflags` -o $@

./objs/src002.o: ./source/src002.cpp  ./source/edgemaker.cpp ./source/edgemaker.hpp
	$(CPPFLAGS) -g -c -std=c++14 -L/usr/local/lib/ ./source/src002.cpp -lpng `libpng-config --cflags` -o $@

./objs/src001.o : ./source/linemanager.hpp ./source/line.hpp ./source/src001.cpp
	$(CPPFLAGS) -g -c -std=c++14 -L/usr/local/lib/ ./source/src001.cpp -lpng `libpng-config --cflags` -o $@

./objs/line.o : ./source/line.hpp ./source/line.cpp
	$(CPPFLAGS) -g -c -std=c++14 -L/usr/local/lib/ ./source/line.cpp -lpng `libpng-config --cflags` -o $@

./objs/linemanager.o : ./source/linemanager.hpp ./source/linemanager.cpp
	$(CPPFLAGS) -g -c -std=c++14 -L/usr/local/lib/ ./source/linemanager.cpp -lpng `libpng-config --cflags` -o $@

./objs/edgemaker.o: ./source/edgemaker.hpp ./source/edgemaker.cpp
	$(CPPFLAGS) -g -c -std=c++14 -L/usr/local/lib ./source/edgemaker.cpp -lpng `libpng-config --cflags` -o $@