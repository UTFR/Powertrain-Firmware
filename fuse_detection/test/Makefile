CC = g++
FLAGS = -fpermissive -ggdb



test: Matrix.o test_driver.o fuse_detection.o
	$(CC) -o test.exe Matrix.o test_driver.o fuse_detection.o

matrix-test: clean Matrix.o matrix_test.o
	$(CC) $(FLAGS) -o test.exe Matrix.o matrix_test.o

clean:
	rm -rf *.o *.exe

valgrind: test.exe
	valgrind --leak-check=full ./test.exe

.PHONY: test matrix-test clean valgrind



Matrix.o: Matrix.hpp Matrix.cpp
	$(CC) $(FLAGS) -c -o Matrix.o Matrix.cpp

matrix_test.o: matrix_test.cpp Matrix.hpp
	$(CC) $(FLAGS) -c -o matrix_test.o matrix_test.cpp

test_driver.o: test_driver.cpp
	$(CC) $(FLAGS) -c -o test_driver.o test_driver.cpp

fuse_detection.o: fuse_detection.cpp
	$(CC) $(FLAGS) -c -o fuse_detection.o fuse_detection.cpp
