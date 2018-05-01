#pragma once


#define EXTENDED_IMPLEMENTATION_LHMATRIX

template <typename T, unsigned int M, unsigned int N>
LhMatrix<T, M, N>::LhMatrix() {
    for (unsigned int line = 0; line < _line; line++) {
        for (unsigned int col = 0; col < _col; col++) {
            _datas[line][col] = 0;
        }
    }
}

template <typename T, unsigned int M, unsigned int N>
LhMatrix<T, M, N>::LhMatrix(LhMatrix<T, M, N> const & other) {
    for (unsigned int line = 0; line < _line; line++) {
        for (unsigned int col = 0; col < _col; col++) {
            _datas[line][col] = other._datas[line][col];
        }
    }
}

template <typename T, unsigned int M, unsigned int N>
LhMatrix<T, M, N>& LhMatrix<T, M, N>::operator=(LhMatrix<T, M, N> const & other) {
    for (unsigned int line = 0; line < _line; line++) {
        for (unsigned int col = 0; col < _col; col++) {
            _datas[line][col] = other._datas[line][col];
        }
    }
}
