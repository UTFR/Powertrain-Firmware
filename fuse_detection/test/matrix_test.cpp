#include <iostream>
#include "Matrix.hpp"

int main(){
	Matrix a(3,3);
	Matrix b(3,3);
	Matrix c = a * b;
	a = b;
	a = b + c;
	bool d = a == b;
	d = a != b;
	a.cell(1,2) = 3;
	b.cell(2,3) = 4;
	c = a * b;
}
