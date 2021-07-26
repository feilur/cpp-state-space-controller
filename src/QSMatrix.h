/**
 * @file QSMatrix.h
 * @brief QSMatrix class header.
 * @author quantstart.com, updated by Alexis Proux
 * @date 1 July 2021
 ******/


#ifndef __QS_MATRIX_H
#define __QS_MATRIX_H

#include <vector>
#include <iostream>
#include <string>  
#include <sstream>   
#include <cmath> 

/**
 * @class QSMatrix
 * @brief Class for matrices and vectors computation.
 ******/
template <typename T> 
class QSMatrix {
 private:
  std::vector<std::vector<T> > mat;
  unsigned rows;
  unsigned cols;

 public:
  QSMatrix();
  QSMatrix(unsigned _rows, unsigned _cols, const T& _initial);
  QSMatrix(const QSMatrix<T>& rhs);
  virtual ~QSMatrix();

  // Operator overloading, for "standard" mathematical matrix operations                                                                                                                                                          
  QSMatrix<T>& operator=(const QSMatrix<T>& rhs);

  // Matrix mathematical operations                                                                                                                                                                                               
  QSMatrix<T> operator+(const QSMatrix<T>& rhs) const;
  QSMatrix<T>& operator+=(const QSMatrix<T>& rhs) const;
  QSMatrix<T> operator-(const QSMatrix<T>& rhs) const;
  QSMatrix<T>& operator-=(const QSMatrix<T>& rhs) const;
  QSMatrix<T> operator*(const QSMatrix<T>& rhs) const;
  QSMatrix<T>& operator*=(const QSMatrix<T>& rhs) const;
  QSMatrix<T> transpose();

  // Matrix/scalar operations                                                                                                                                                                                                     
  QSMatrix<T> operator+(const T& rhs) const;
  QSMatrix<T> operator-(const T& rhs) const;
  QSMatrix<T> operator*(const T& rhs) const;
  QSMatrix<T> operator/(const T& rhs) const;

  // Matrix/vector operations                                                                                                                                                                                                     
  std::vector<T> operator*(const std::vector<T>& rhs) const;
  std::vector<T> diag_vec();
  
  // Vector/vector operations
  static std::vector<T> vectorAdd(const std::vector<T>& vec1, const std::vector<T>& vec2);
  static std::vector<T> vectorSubstract(const std::vector<T>& vec1, const std::vector<T>& vec2);

  // Access the individual elements                                                                                                                                                                                               
  T& operator()(const unsigned& row, const unsigned& col);
  const T& operator()(const unsigned& row, const unsigned& col) const;

  // Access the row and column sizes                                                                                                                                                                                              
  unsigned get_rows() const;
  unsigned get_cols() const;
  
  void print() const;
  std::string getRepresentation() const;
};

#include "QSMatrix.cpp"

#endif
