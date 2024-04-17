#pragma once
#include <iostream>

using namespace std;

class Matrix {
  private:
	class Mref;
	struct rcmatrix;

	bool compareSize(const Matrix& m) const;

  public:
	rcmatrix* matrix;

	Matrix();
	Matrix(int r, int c);
	Matrix(const Matrix& m);

	~Matrix();

	int getCol() const;
	int getRow() const;

	void operator=(const Matrix& m);
	friend bool operator==(const Matrix& lhs, const Matrix& rhs);
	friend bool operator!=(const Matrix& lhs, const Matrix& rhs);

	Mref operator()(int x, int y);
	double operator()(int x, int y) const;

	Matrix& operator+=(const Matrix& m);
	Matrix& operator-=(const Matrix& m);
	friend Matrix operator+(const Matrix& lhs, const Matrix& rhs);
	friend Matrix operator-(const Matrix& lhs, const Matrix& rhs);

	friend istream& operator>>(istream& stream, Matrix& m);
	friend std::ostream& operator<<(ostream& stream, const Matrix& m);

	class MatrixSize;
	class IncorrectInputType;
	class IndexOutOfRange;
};

class Matrix::MatrixSize{
  public:
	const char* what() const throw();
};

class Matrix::IncorrectInputType {
  public:
	const char* what() const throw();
};

class Matrix::IndexOutOfRange {
  public:
	const char* what() const throw();
};

struct Matrix::rcmatrix {
	double** matrixData;
	int referenceCount;
	int rows, columns;
	rcmatrix(int rows = 0, int columns = 0);
	~rcmatrix();
	rcmatrix* detach();
};

class Matrix::Mref {
  public:
	operator double() const;
	Matrix::Mref& operator=(double value);
	Matrix::Mref& operator=(const Mref& reference);

  private:
	friend class Matrix;
	const Matrix& mtrx;
	int row, col;
	Mref(const Matrix& matrix, int row, int col)
		: mtrx(matrix), row(row), col(col){};
};
