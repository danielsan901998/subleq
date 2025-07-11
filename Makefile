CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11

all: subleq

subleq: main.cpp subleq.hpp
	$(CXX) $(CXXFLAGS) main.cpp -o subleq

.PHONY: clean

test: tests/test.cpp subleq.hpp
	$(CXX) $(CXXFLAGS) tests/test.cpp -o test_subleq
	./test_subleq

clean:
	rm -f subleq test_subleq
