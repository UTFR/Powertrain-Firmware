CC = g++
FLAGS = -fpermissive

test.exe: Matrix.o matrix_test.o
	$(CC) -o test.exe Matrix.o matrix_test.o

Matrix.o: Matrix.hpp Matrix.cpp
	$(CC) $(FLAGS) -c -o Matrix.o Matrix.cpp

matrix_test.o: matrix_test.cpp Matrix.hpp
	$(CC) $(FLAGS) -c -o matrix_test.o matrix_test.cpp

clean:
	rm -rf *.o *.exe

valgrind: test.exe
	valgrind --leak-check=full ./test.exe

