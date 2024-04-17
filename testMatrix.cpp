#include "Matrix.h"
#include <fstream>
#include <iostream>

void TestWriteRead();
void TestAdd();
void TestSub();
void TestExceptions();

using namespace std;

int main()
{
	try {
		TestWriteRead();
		TestAdd();
		TestSub();
		TestExceptions();
	}
	catch (Matrix::MatrixSize& e) {
		cerr << e.what() << endl;
	}
	catch (Matrix::IncorrectInputType& e) {
		cerr << e.what() << endl;
	}
	catch (Matrix::IndexOutOfRange& e) {
		cerr << e.what() << endl;
	}

	return 0;
}

void TestExceptions()
{
	try {
		Matrix m1(1, 1);
		Matrix m2(2, 3);
		Matrix m3(1, 1);

		ifstream sample_file("sample_matrix.txt");
		ifstream sample_file2("sample_matrix2.txt");
		ifstream sample_file3("sample_matrix3.txt");

		sample_file >> m1;
		sample_file2 >> m2;
		sample_file3 >> m3;

		m3 = m1 + m2;

		/*m1 = m2;

		m1(5,5) = 0;*/

		sample_file.close();
		sample_file2.close();
		sample_file3.close();
	}
	catch (Matrix::MatrixSize& e) {
		cerr << e.what() << endl;
	}
	catch (Matrix::IncorrectInputType& e) {
		cerr << e.what() << endl;
	}
	catch (Matrix::IndexOutOfRange& e) {
		cerr << e.what() << endl;
	}
}

void TestWriteRead()
{
	Matrix m1(1, 1);
	Matrix m2(1, 1);

	ifstream sample_file("sample_matrix.txt");
	ifstream sample_file2("sample_matrix2.txt");

	sample_file >> m1;
	sample_file2 >> m2;

	cout << "m1: " << endl << m1 << endl;
	cout << "m2: " << endl << m2 << endl;

	m2(0, 0) = 1;
	m2(0, 1) = 1;
	m2(1, 0) = 1;
	m2(1, 1) = 1;

	double a = m2(0, 0);
	cout << "m2(0,0): " << a << endl;

	cout << "m1: " << endl << m1 << endl;
	cout << "m2: " << endl << m2 << endl;

	sample_file.close();
	sample_file2.close();
}

void TestAdd()
{
	Matrix m1(1, 1);
	Matrix m2(1, 1);
	Matrix m3(3, 3);

	ifstream sample_file("sample_matrix.txt");
	sample_file >> m1;

	ifstream sample_file2("sample_matrix.txt");
	sample_file2 >> m2;

	m3 = m1 + m2;
	cout << "m3 = m1 + m2: " << endl << m3 << endl;
	m1 += m2;
	cout << "m1 += m2: " << endl << m1 << endl;

	sample_file.close();
	sample_file2.close();
}

void TestSub()
{

	Matrix m1(1, 1);
	Matrix m2(1, 1);
	Matrix m3(3, 3);

	ifstream sample_file("sample_matrix.txt");
	sample_file >> m1;

	ifstream sample_file2("sample_matrix.txt");
	sample_file2 >> m2;

	m3 = m1 - m2;
	cout << "m3 = m1 - m2: " << endl << m3 << endl;
	m1 -= m2;
	cout << "m1 -= m2: " << endl << m1 << endl;

	sample_file.close();
	sample_file2.close();
}
