CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -pthread -I../../project

SRCS = time_test.cpp ../../project/Time.cpp
OBJS = $(SRCS:.cpp=.o)

GTEST_LIBS = -lgtest -lgtest_main -pthread

all: run_tests

run_tests: $(OBJS)
	$(CXX) $(CXXFLAGS) -o run_tests $(OBJS) $(GTEST_LIBS)

clean:
	rm -f $(OBJS) run_tests
