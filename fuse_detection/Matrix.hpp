/*
 * Matrix library for the ATMEGA2560
 * Copyright 2021 UTFR
 */

#pragma once
#include <stdint.h>

class Matrix {
private:
  uint16_t**data;
  int num_rows, num_cols;
  bool failed;
public:
  //Constructors
  Matrix(const int &num_rows, const int &num_cols);
  Matrix(const int &num_rows, const int &num_cols, uint16_t**data);
  Matrix(const Matrix& m);
  //Destructor
  virtual ~Matrix();

  //Overloaded operators
  Matrix & operator+(const Matrix &rhs);
  Matrix & operator-(const Matrix &rhs);
  Matrix & operator*(const Matrix &rhs);
  Matrix & operator*(const uint16_t &i);
  Matrix & operator=(const Matrix &rhs);

  bool operator==(const Matrix &rhs);
  bool operator!=(const Matrix &rhs);

  //Get reference to cell
  uint16_t & cell(int row, int col);

  //Get number of rows and collumns
  int rows();
  int cols();

  //See if the memory allocation failed. Will probably be removed in the actual product
  bool has_failed();
};
