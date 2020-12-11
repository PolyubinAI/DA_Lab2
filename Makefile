CXX = g++
FLAGS = -std=c++17 -pedantic -g -Wall -Werror -Wno-sign-compare -Wno-long-long -Ofast
SOURCES = main.o
EXECUTABLE = solution
all: build

gprof: $(SOURCES)
	$(CXX) $(FLAGS) -pg -o start_gp $(SOURCES)

build: $(SOURCES)
	$(CXX) $(FLAGS) -o $(EXECUTABLE) $(SOURCES)

main.o: main.cpp
	$(CXX) $(FLAGS) -c main.cpp

TPatricia.o: TPatriciaNode.hpp TPatricia.hpp
	$(CXX) $(FLAGS) -c TPatricia.cpp

clean:
	rm -f $(SOURCES) $(EXECUTABLE)