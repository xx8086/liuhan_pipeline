#pragma once
#include <assert.h>

//namespace lh_pipeline {
#define EXTENDED_IMPLEMENTATION_LHVERTEX

    template <typename T, unsigned int N>
    LhVertex<T, N>::LhVertex() {
        for (int i = 0; i < _counts; i++) {
            _datas[i] = 0;
        }
    }

    template <typename T, unsigned int N>//ͬ����ͬʵ������
    LhVertex<T, N>::LhVertex(LhVertex<T, N> const & other) {
        for (int i = 0; i < _counts; i++) {
            _datas[i] = other._datas[i];
        }
    }

    template <typename T, unsigned int N>//ͬ����ͬʵ����ֵ
    LhVertex<T, N>& LhVertex<T, N>::operator=(LhVertex<T, N> const & other) {
        for (int i = 0; i < _counts; i++) {
            _datas[i] = other._datas[i];
        }
    }
/*
    template <typename T, unsigned int N>
    template<unsigned int M>//ͬ���Ͳ�ͬʵ������
    LhVertex<T, N>::LhVertex(LhVertex<T, M> const & other) {
        int counts = _counts <= other._counts ? _counts : other._counts;
        for (int i = 0; i < counts; i++) {
            _datas[i] = other._datas[i];
        }
    }

    template <typename T, unsigned int N>
    template<typename U>//��ͬ����ͬʵ������
    LhVertex<T, N>::LhVertex(LhVertex<U, N> const & other) {
        for (int i = 0; i < _counts; i++) {
            _datas[i] = static_cast<T>(other._datas[i]);
        }
    }

    template <typename T, unsigned int N>
    template<typename U, unsigned int M>//��ͬ���Ͳ�ͬʵ������
    LhVertex<T, N>::LhVertex(LhVertex<U, M> const & other) {
        int counts = _counts <= other._counts ? _counts : other._counts;
        for (int i = 0; i < counts; i++) {
            _datas[i] = static_cast<T>(other._datas[i]);
        }
    }

    template <typename T, unsigned int N>
    template<unsigned int M>//ͬ���Ͳ�ͬʵ����ֵ
    LhVertex<T, N>::LhVertex<T, N>& operator=(LhVertex<T, M> const & other) {
        int counts = _counts <= other._counts ? _counts : other._counts;
        for (int i = 0; i < counts; i++) {
            _datas[i] = other._datas[i];
        }
    }

    template <typename T, unsigned int N>
    template<typename U>//��ͬ����ͬʵ����ֵ
    LhVertex<T, N>::LhVertex<T, N>& operator=(LhVertex<U, N> const & other) {
        for (int i = 0; i < _counts; i++) {
            _datas[i] = static_cast<T>(other._datas[i]);
        }
    }

    template <typename T, unsigned int N>
    template<typename U, unsigned int M>//��ͬ���Ͳ�ͬʵ����ֵ
    LhVertex<T, N>::LhVertex<T, N>& operator=(LhVertex<U, M> const & other) {
        int counts = _counts <= other._counts ? _counts : other._counts;
        for (int i = 0; i < counts; i++) {
            _datas[i] = static_cast<T>(other._datas[i]);
        }
    }
*/
    template <typename T, unsigned int N>
    T LhVertex<T, N>::dot(LhVertex<T, N> const & other) {
        assert(counts > 0);
        T t = _datas[0] * other._datas[0];
        for (int i = 1; i < _counts; i++) {
            t += _datas[i] * other._datas[i];
        }
        return t;
    }

    template <typename T, unsigned int N>
    bool LhVertex<T, N>::normalize() {
        const float length = sqrtf(static_cast<float>dot(*this));
        if (length > 0.00001) {
            for (int i = 1; i < _counts; i++) {
                _datas[i] /= length;
            }
        }
        else {
            return false;
        }
        return true;
    }
//}
