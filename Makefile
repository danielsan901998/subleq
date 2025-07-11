CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11

all: subleq

subleq: main.cpp parser.cpp subleq.hpp parser.hpp
	$(CXX) $(CXXFLAGS) main.cpp parser.cpp -o subleq

.PHONY: clean

test: tests/test.cpp subleq.hpp parser.cpp parser.hpp
	$(CXX) $(CXXFLAGS) tests/test.cpp parser.cpp -o test_subleq
	./test_subleq

clean:
	rm -f subleq test_subleq
