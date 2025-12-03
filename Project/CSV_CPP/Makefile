CC = gcc
CXX = g++
CFLAGS = -Wall -Wextra -std=c11
CXXFLAGS = -Wall -Wextra -std=c++17

all: cpp_test cross_test

cpp_test: CSV.cpp CSV_test.cpp
	$(CXX) $(CXXFLAGS) -o main_cpp CSV.cpp CSV_test.cpp

cross_test: CSV.cpp CSV_cross_test.cpp ../CSV_C/CSV.c
	$(CC) $(CFLAGS) -c ../CSV_C/CSV.c -o ../CSV_C/CSV.o
	$(CXX) $(CXXFLAGS) -o cross_check CSV_cross_test.cpp CSV.cpp ../CSV_C/CSV.o

clean:
	rm -f main_cpp cross_check *.o *.csv ../CSV_C/*.o