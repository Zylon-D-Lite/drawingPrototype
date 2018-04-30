CXX=g++
LIBPNGPATH =/usr/local/lib/
LIBPNG=png
LIBPNGFLAGS=`libpng-config --cflags`
OUT_DIR= ./objs/ ./debug/ ./examples/
MKDIR_P= mkdir -p
.PHONY : directories

all: directories ./examples/test001 ./examples/test002 ./examples/test003 ./debug/debug ./examples/delaunay

directories: ${OUT_DIR}

${OUT_DIRS} :
	${MKDIR_P} ${OUT_DIR}

./examples/delaunay: ./objs/triangluation-test.o ./objs/delaunay.o ./objs/artist.o
	$(CXX) -g -std=c++14 -L$(LIBPNGPATH) $^ -l$(LIBPNG) $(LIBPNGFLAGS) -o $@

./objs/triangluation-test.o : ./source/triangulation-test.cpp ./objs/delaunay.o ./objs/triangle.o ./objs/artist.o
	$(CXX) -g -c -std=c++14 $< -o $@

./debug/debug: ./objs/debug.o ./objs/delaunay.o ./objs/triangle.o ./objs/artist.o
	$(CXX) -g -std=c++14 -L$(LIBPNGPATH) $^ -l$(LIBPNG) $(LIBPNGFLAGS) -o $@

./objs/artist.o : ./source/artist.cpp ./source/artist.hpp
	$(CXX) -g -c -std=c++14 $< -o $@

./objs/debug.o : ./source/debug.cpp ./objs/delaunay.o ./objs/triangle.o ./objs/artist.o
	$(CXX) -g -c -std=c++14  $<  -o $@

./objs/triangle.o : ./source/triangle.cpp ./source/triangle.hpp
	$(CXX) -g -c -std=c++14 $< -o $@

./objs/delaunay.o : ./source/delaunay.cpp ./source/delaunay.hpp ./source/triangle.hpp
	$(CXX) -g -c -std=c++14  $<  -o $@

./examples/test001: ./source/main.cpp
	$(CXX) -g -std=c++14 -L$(LIBPNGPATH) $< -lpng -o $@

./examples/test002: ./objs/src001.o ./objs/line.o ./objs/linemanager.o ./objs/edgemaker.o
	$(CXX) -g -std=c++14 -L$(LIBPNGPATH) $^ -l$(LIBPNG) $(LIBPNGFLAGS) -o $@

./examples/test003: ./objs/src002.o ./objs/edgemaker.o ./objs/linemanager.o ./objs/line.o
	$(CXX) -g -std=c++14 -L$(LIBPNGPATH) $^ -l$(LIBPNG) $(LIBPNGFLAGS) -o $@

./objs/src001.o : ./source/src001.cpp ./source/linemanager.hpp ./source/line.hpp
	$(CXX) -g -c -std=c++14 -L$(LIBPNGPATH) $< -l$(LIBPNG) $(LIBPNGFLAGS) -o $@

./objs/src002.o: ./source/src002.cpp  ./source/edgemaker.cpp ./source/edgemaker.hpp
	$(CXX) -g -c -std=c++14 -L$(LIBPNGPATH) ./source/src002.cpp -l$(LIBPNG) $(LIBPNGFLAGS) -o $@

./objs/line.o : ./source/line.cpp ./source/line.hpp
	$(CXX) -g -c -std=c++14 -L$(LIBPNGPATH) $< -l$(LIBPNG) $(LIBPNGFLAGS) -o $@

./objs/linemanager.o : ./source/linemanager.cpp ./source/linemanager.hpp
	$(CXX) -g -c -std=c++14 -L$(LIBPNGPATH) $< -l$(LIBPNG) $(LIBPNGFLAGS) -o $@

./objs/edgemaker.o: ./source/edgemaker.cpp ./source/edgemaker.hpp
	$(CXX) -g -c -std=c++14 -L$(LIBPNGPATH) $< -l$(LIBPNG) $(LIBPNGFLAGS) -o $@

clean :
	rm ./objs/*