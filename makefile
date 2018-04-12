CPPFLAGS=g++
LIBPNGLIBPATH =/usr/local/lib/
LIBPNGINCLUDEPATH=png
LIBPNGFLAGS=`libpng-config --cflags`

project: ./examples/test001 ./examples/test002 ./examples/test003

./examples/test001: ./source/main.cpp
	$(CPPFLAGS) -g -std=c++14 -L$(LIBPNGLIBPATH) $< -lpng -o $@

./examples/test002: ./objs/src001.o ./objs/line.o ./objs/linemanager.o ./objs/edgemaker.o
	$(CPPFLAGS) -g -std=c++14 -L$(LIBPNGLIBPATH) $^ -l$(LIBPNGINCLUDEPATH) $(LIBPNGFLAGS) -o $@

./examples/test003: ./objs/src002.o ./objs/edgemaker.o ./objs/linemanager.o ./objs/line.o
	$(CPPFLAGS) -g -std=c++14 -L$(LIBPNGLIBPATH) $^ -l$(LIBPNGINCLUDEPATH) $(LIBPNGFLAGS) -o $@

./objs/src001.o : ./source/src001.cpp ./source/linemanager.hpp ./source/line.hpp
	$(CPPFLAGS) -g -c -std=c++14 -L$(LIBPNGLIBPATH) $< -l$(LIBPNGINCLUDEPATH) $(LIBPNGFLAGS) -o $@

./objs/src002.o: ./source/src002.cpp  ./source/edgemaker.cpp ./source/edgemaker.hpp
	$(CPPFLAGS) -g -c -std=c++14 -L$(LIBPNGLIBPATH) ./source/src002.cpp -l$(LIBPNGINCLUDEPATH) $(LIBPNGFLAGS) -o $@

./objs/line.o : ./source/line.cpp ./source/line.hpp
	$(CPPFLAGS) -g -c -std=c++14 -L$(LIBPNGLIBPATH) $< -l$(LIBPNGINCLUDEPATH) $(LIBPNGFLAGS) -o $@

./objs/linemanager.o : ./source/linemanager.cpp ./source/linemanager.hpp
	$(CPPFLAGS) -g -c -std=c++14 -L$(LIBPNGLIBPATH) $< -l$(LIBPNGINCLUDEPATH) $(LIBPNGFLAGS) -o $@

./objs/edgemaker.o: ./source/edgemaker.cpp ./source/edgemaker.hpp
	$(CPPFLAGS) -g -c -std=c++14 -L$(LIBPNGLIBPATH) $< -l$(LIBPNGINCLUDEPATH) $(LIBPNGFLAGS) -o $@

clean :
	rm ./objs/*