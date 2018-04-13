#pragma once

template <typename T, unsigned int M, unsigned int N>
LhMatrix<T, M, N>& operator+(LhMatrix<T, M, N> const & left, LhMatrix<T, M, N> const& right) {
    LhMatrix<T, M, N> temp;
    for (unsigned int line = 0; line < left._line; line++) {
        for (unsigned int col = 0; col < left._col; col++) {
            temp._datas[line][col] = left._datas[line][col] + right._datas[line][col];
        }
    }
    return temp;
}

template <typename T, unsigned int M, unsigned int N>
LhMatrix<T, M, N>& operator-(LhMatrix<T, M, N> const & left, LhMatrix<T, M, N> const& right) {
    LhMatrix<T, M, N> temp;
    for (unsigned int line = 0; line < left._line; line++) {
        for (unsigned int col = 0; col < left._col; col++) {
            temp._datas[line][col] = left._datas[line][col] - right._datas[line][col];
        }
    }
    return temp;
}



template <typename T, unsigned int M, unsigned int N>
LhMatrix<T, M, N>& operator*(LhMatrix<T, M, N> const & left, LhMatrix<T, M, N> const& right) {
    LhMatrix<T, M, N> temp;
    for (int i = 0; i < temp._counts; i++) {
        temp._datas[i] = left._datas[i] * right._datas[i];
    }
    return temp;
}

template <typename T, unsigned int M, unsigned int N>
LhMatrix<T, M, N>& operator/(LhMatrix<T, M, N> const & left, LhMatrix<T, M, N> const& right) {
    LhMatrix<T, M, N> temp;
    for (int i = 0; i < temp._counts; i++) {
        temp._datas[i] = left._datas[i] / right._datas[i];
    }
    return temp;
}



template <typename T, unsigned int M, unsigned int N>
LhMatrix<T, M, N>& operator+ (LhMatrix<T, M, N> const & left, const T& scalar) {
    LhMatrix<T, M, N> temp;
    for (int i = 0; i < temp._counts; i++) {
        temp._datas[i] = left._datas[i] + scalar;
    }
    return temp;
}

template <typename T, unsigned int M, unsigned int N>
LhMatrix<T, M, N>& operator- (LhMatrix<T, M, N> const & left, const T& scalar) {
    LhMatrix<T, M, N> temp;
    for (int i = 0; i < temp._counts; i++) {
        temp._datas[i] = left._datas[i] - scalar;
    }
    return temp;
}

template <typename T, unsigned int M, unsigned int N>
LhMatrix<T, M, N>& operator* (LhMatrix<T, M, N> const & left, const T& scalar) {
    LhMatrix<T, M, N> temp;
    return temp;
}

template <typename T, unsigned int M, unsigned int N>
LhMatrix<T, M, N>& operator/ (LhMatrix<T, M, N> const & left, const T& scalar) {
    LhMatrix<T, M, N> temp;
    return temp;
}


template <typename T, unsigned int M, unsigned int N>
bool operator==(LhMatrix<T, M, N> const & left, LhMatrix<T, M, N> const& right) {
    bool rt = true;
    for (unsigned int line = 0; line < left._line; line++) {
        for (unsigned int col = 0; col < left._col; col++) {
            if (left._datas[line][col] != right._datas[line][col]) {
                rt = false;
                break;
            };
        }
    }
    return rt;
}

template <typename T, unsigned int M, unsigned int N>
bool operator!=(LhMatrix<T, M, N> const & left, LhMatrix<T, M, N> const& right) {
    bool rt = false;
    for (unsigned int line = 0; line < left._line; line++) {
        for (unsigned int col = 0; col < left._col; col++) {
            if (left._datas[line][col] != right._datas[line][col]){
                rt = true;
                break;
            };
        }
    }
    return rt;
}