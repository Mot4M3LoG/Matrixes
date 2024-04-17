#include "Matrix.h"
#include <iostream>

using namespace std;

Matrix::Matrix()
{
	this->matrix = nullptr;
}

Matrix::Matrix(int rows, int columns)
{
	rcmatrix* rcm = new rcmatrix(rows, columns);
	this->matrix = rcm;
}

Matrix::~Matrix()
{
	if (this->matrix != nullptr) {
		this->matrix->referenceCount--;
		if (!this->matrix->referenceCount)
			delete this->matrix;
		else
			this->matrix = nullptr;
	}
}

Matrix::Matrix(const Matrix& other)
{
	this->matrix = other.matrix;
	other.matrix->referenceCount++;
}

void Matrix::operator=(const Matrix& m)
{
	m.matrix->referenceCount++;
	if (--this->matrix->referenceCount == 0)
		delete this->matrix;
	this->matrix = m.matrix;
}

istream& operator>>(istream& stream, Matrix& m)
{
	int r, c;
	if (!(stream >> r) || !(stream >> c)) {
		throw Matrix::IncorrectInputType();
	}

	Matrix::rcmatrix* rcm;
	rcm = new Matrix::rcmatrix(r, c);

	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			if (!(stream >> rcm->matrixData[i][j])) {
				throw Matrix::IncorrectInputType();
			}
		}
	}
	if (m.matrix->referenceCount == 1)
		delete m.matrix;
	else {
		m.matrix = nullptr;
	}

	m.matrix = rcm;
	return stream;
}

ostream& operator<<(ostream& os, Matrix const& m)
{
	for (int i = 0; i < m.getRow(); i++) {
		for (int j = 0; j < m.getCol(); j++) {
			os << m.matrix->matrixData[i][j] << ' ';
		}
		os << endl;
	}
	return os;
}

int Matrix::getCol() const
{
	return this->matrix->columns;
}

int Matrix::getRow() const
{
	return this->matrix->rows;
}

Matrix::rcmatrix::rcmatrix(int rows, int columns)
{
	this->rows = rows;
	this->columns = columns;
	this->referenceCount = 1;

	this->matrixData = new double* [rows] {};

	for (int i = 0; i < rows; i++) {
		this->matrixData[i] = new double[columns]{};
	}
}

Matrix::rcmatrix::~rcmatrix()
{
	for (int i = 0; i < rows; i++) {
		delete[] matrixData[i];
	}
	delete[] matrixData;
}

Matrix::rcmatrix* Matrix::rcmatrix::detach()
{
	if (referenceCount == 1) {
		return this;
	}
	rcmatrix* m;
	rcmatrix* temp_m = new rcmatrix(rows, columns);
	m = temp_m;

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < rows; j++) {
			m->matrixData[i][j] = this->matrixData[i][j];
		}
	}
	this->referenceCount--;
	return m;
}

double Matrix::operator()(int row, int col) const
{
	return Mref(*this, row, col);
}

Matrix::Mref Matrix::operator()(int row, int col)
{
	if (this->getCol() < col || this->getRow() < row) {
		throw Matrix::IndexOutOfRange();
	}

	return Mref(*this, row, col);
}

Matrix::Mref::operator double() const
{ // read
	return this->mtrx.matrix->matrixData[this->row][this->col];
}

Matrix::Mref& Matrix::Mref::operator=(double value)
{ // write
	this->mtrx.matrix->matrixData[this->row][this->col] = value;
	return *this;
}

Matrix::Mref& Matrix::Mref::operator=(const Mref& ref)
{ // convert mref to double and call =
	return operator=((double)ref);
}

bool Matrix::compareSize(const Matrix& m) const
{
	return (this->matrix->columns == m.matrix->columns &&
			this->matrix->rows == m.matrix->rows);
}

bool operator==(const Matrix& lhs, const Matrix& rhs)
{
	if (lhs.compareSize(rhs)) {
		bool comparison = true;
		for (int i = 0; i < lhs.getRow() && comparison; i++) {
			for (int j = 0; j < lhs.getCol() && comparison; j++) {
				if (lhs.matrix->matrixData[i][j] !=
					rhs.matrix->matrixData[i][j]) {
					comparison = false;
				}
			}
		}
		return comparison;
	}
	else {
		return false;
	}
}
bool operator!=(const Matrix& lhs, const Matrix& rhs)
{
	return (!(lhs == rhs));
}

Matrix& Matrix::operator+=(const Matrix& rhs)
{
	if (this->compareSize(rhs)) {
		this->matrix = this->matrix->detach();
		for (int i = 0; i < this->getRow(); i++) {
			for (int j = 0; j < this->getCol(); j++) {
				this->matrix->matrixData[i][j] += rhs.matrix->matrixData[i][j];
			}
		}
	}
	else {
		throw Matrix::MatrixSize();
	}

	return *this;
}

Matrix operator+(const Matrix& lhs, const Matrix& rhs)
{
	if (lhs.compareSize(rhs))
		return Matrix(lhs) += rhs;
	else
		throw Matrix::MatrixSize();
}

Matrix& Matrix::operator-=(const Matrix& rhs)
{
	if (this->compareSize(rhs)) {
		this->matrix = this->matrix->detach();
		for (int i = 0; i < this->getRow(); i++) {
			for (int j = 0; j < this->getCol(); j++) {
				this->matrix->matrixData[i][j] -= rhs.matrix->matrixData[i][j];
			}
		}
	}
	else {
		throw Matrix::MatrixSize();
	}

	return *this;
}

Matrix operator-(const Matrix& lhs, const Matrix& rhs)
{
	if (lhs.compareSize(rhs))
		return Matrix(lhs) -= rhs;
	else
		throw Matrix::MatrixSize();
}

const char* Matrix::MatrixSize::what() const throw()
{
	return "Cannot operate on different sizes";
}

const char* Matrix::IncorrectInputType::what() const throw()
{
	return "Wrong type of the input data";
}

const char* Matrix::IndexOutOfRange::what() const throw()
{
	return "Index not found";
}
