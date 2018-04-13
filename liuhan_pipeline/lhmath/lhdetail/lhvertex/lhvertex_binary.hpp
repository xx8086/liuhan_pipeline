#pragma once

//namespace lh_pipeline {
#define EXTENDED_IMPLEMENTATION_BINARY_LHVERTEX

    template <typename T, unsigned int N>
    LhVertex<T, N>& operator+(LhVertex<T, N> const & left, LhVertex<T, N> const& right) {
        LhVertex<T, N> temp;
        for (int i = 0; i < temp._counts; i++) {
            temp._datas[i] = left._datas[i] + right._datas[i];
        }
        return temp;
    }

    template <typename T, unsigned int N>
    LhVertex<T, N>& operator-(LhVertex<T, N> const & left, LhVertex<T, N> const& right) {
        LhVertex<T, N> temp;
        for (int i = 0; i < temp._counts; i++) {
            temp._datas[i] = left._datas[i] - right._datas[i];
        }
        return temp;
    }



    template <typename T, unsigned int N>
    LhVertex<T, N>& operator*(LhVertex<T, N> const & left, LhVertex<T, N> const& right) {
        LhVertex<T, N> temp;
        for (int i = 0; i < temp._counts; i++) {
            temp._datas[i] = left._datas[i] * right._datas[i];
        }
        return temp;
    }

    template <typename T, unsigned int N>
    LhVertex<T, N>& operator/(LhVertex<T, N> const & left, LhVertex<T, N> const& right) {
        LhVertex<T, N> temp;
        for (int i = 0; i < temp._counts; i++) {
            temp._datas[i] = left._datas[i] / right._datas[i];
        }
        return temp;
    }



    template <typename T, unsigned int N>
    LhVertex<T, N>& operator+ (LhVertex<T, N> const & left, const T& scalar) {
        LhVertex<T, N> temp;
        for (int i = 0; i < temp._counts; i++) {
            temp._datas[i] = left._datas[i] + scalar;
        }
        return temp;
    }

    template <typename T, unsigned int N>
    LhVertex<T, N>& operator- (LhVertex<T, N> const & left, const T& scalar) {
        LhVertex<T, N> temp;
        for (int i = 0; i < temp._counts; i++) {
            temp._datas[i] = left._datas[i] - scalar;
        }
        return temp;
    }

    template <typename T, unsigned int N>
    LhVertex<T, N>& operator* (LhVertex<T, N> const & left, const T& scalar) {
        LhVertex<T, N> temp;
        for (int i = 0; i < temp._counts; i++) {
            temp._datas[i] = left._datas[i] * scalar;
        }
        return temp;
    }

    template <typename T, unsigned int N>
    LhVertex<T, N>& operator/ (LhVertex<T, N> const & left, const T& scalar) {
        LhVertex<T, N> temp;
        for (int i = 0; i < temp._counts; i++) {
            temp._datas[i] = left._datas[i] / scalar;
        }
        return temp;
    }


    template <typename T, unsigned int N>
    bool operator==(LhVertex<T, N> const & left, LhVertex<T, N> const& right) {
        bool rt = true;
        for (int i = 0; i < left._datas; i++) {
            if (temp._datas[i] != left._datas[i]) {
                rt = false;
                break;
            }
        }
        return rt;
    }

    template <typename T, unsigned int N>
    bool operator!=(LhVertex<T, N> const & left, LhVertex<T, N> const& right) {
        bool rt = false;
        for (int i = 0; i < left._datas; i++) {
            if (temp._datas[i] != left._datas[i]) {
                rt = true;
                break;
            }
        }
        return rt;
    }
//}