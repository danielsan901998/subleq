CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -Iinc

all: subleq

subleq: src/main.cpp src/parser.cpp src/subleq.cpp inc/subleq.hpp inc/parser.hpp
	$(CXX) $(CXXFLAGS) src/main.cpp src/parser.cpp src/subleq.cpp -o subleq

.PHONY: clean

test: tests/test.cpp src/parser.cpp src/subleq.cpp inc/subleq.hpp inc/parser.hpp
	$(CXX) $(CXXFLAGS) tests/test.cpp src/parser.cpp src/subleq.cpp -o test_subleq
	./test_subleq

clean:
	rm -f subleq test_subleq
