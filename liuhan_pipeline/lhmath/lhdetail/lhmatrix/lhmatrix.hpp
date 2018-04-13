#pragma once

template <typename T, unsigned int M, unsigned int N>
class LhMatrix {
public:
    unsigned int _lin = M;
    unsigned int _col = N;
    T _datas[M][N];
public:
    ~LhMatrix() {}
    LhMatrix();
    LhMatrix(LhMatrix<T, M, N> const & other);
    LhMatrix<T, M, N>& operator=(LhMatrix<T, M, N> const & other);
};

#ifndef EXTENDED_IMPLEMENTATION_LHMATRIX//实现
#include "lhmatrix.inl"
#endif

#ifndef EXTENDED_IMPLEMENTATION_OPERATOR_LHMATRIX//一些矩阵操作
template <typename T, unsigned int M, unsigned int N>
LhMatrix<T, M, N>& operator+(LhMatrix<T, M, N> const & left, LhMatrix<T, M, N> const& right);//加
template <typename T, unsigned int M, unsigned int N>
LhMatrix<T, M, N>& operator-(LhMatrix<T, M, N> const & left, LhMatrix<T, M, N> const& right);//减
template <typename T, unsigned int M, unsigned int N>
LhMatrix<T, M, N>& operator*(LhMatrix<T, M, N> const & left, LhMatrix<T, M, N> const& right);//乘
template <typename T, unsigned int M, unsigned int N>
LhMatrix<T, M, N>& operator/(LhMatrix<T, M, N> const & left, LhMatrix<T, M, N> const& right);//除

template <typename T, unsigned int M, unsigned int N>
LhMatrix<T, M, N>& operator+(LhMatrix<T, M, N> const & left, const T& scalar);//加
template <typename T, unsigned int M, unsigned int N>
LhMatrix<T, M, N>& operator-(LhMatrix<T, M, N> const & left, const T& scalar);//减
template <typename T, unsigned int M, unsigned int N>
LhMatrix<T, M, N>& operator*(LhMatrix<T, M, N> const & left, const T& scalar);//乘
template <typename T, unsigned int M, unsigned int N>
LhMatrix<T, M, N>& operator/(LhMatrix<T, M, N> const & left, const T& scalar);//除

template <typename T, unsigned int M, unsigned int N>
bool operator==(LhMatrix<T, M, N> const & left, LhMatrix<T, M, N> const& right);//==

template <typename T, unsigned int M, unsigned int N>
bool operator!=(LhMatrix<T, M, N> const & left, LhMatrix<T, M, N> const& right);//!=
#include "lhmatrix_operator.hpp"
#endif