#pragma once

//namespace lh_pipeline {
#define EXTENDED_IMPLEMENTATION_LHVERTEX

	template <typename T, unsigned int N>
	LhVertex<T, N>::LhVertex() {
		for (int i = 0; i < _counts; i++) {
			_datas[i] = 0;
		}
	}

	template <typename T, unsigned int N>//同类型同实数构造
	LhVertex<T, N>::LhVertex(LhVertex<T, N> const & other) {
		for (int i = 0; i < _counts; i++) {
			_datas[i] = other._datas[i];
		}
	}

	template <typename T, unsigned int N>//同类型同实数赋值
	LhVertex<T, N>& LhVertex<T, N>::operator=(LhVertex<T, N> const & other) {
		for (int i = 0; i < _counts; i++) {
			_datas[i] = other._datas[i];
		}
	}
/*
	template <typename T, unsigned int N>
	template<unsigned int M>//同类型不同实数构造
	LhVertex<T, N>::LhVertex(LhVertex<T, M> const & other) {
		int counts = _counts <= other._counts ? _counts : other._counts;
		for (int i = 0; i < counts; i++) {
			_datas[i] = other._datas[i];
		}
	}

	template <typename T, unsigned int N>
	template<typename U>//不同类型同实数构造
	LhVertex<T, N>::LhVertex(LhVertex<U, N> const & other) {
		for (int i = 0; i < _counts; i++) {
			_datas[i] = static_cast<T>(other._datas[i]);
		}
	}

	template <typename T, unsigned int N>
	template<typename U, unsigned int M>//不同类型不同实数构造
	LhVertex<T, N>::LhVertex(LhVertex<U, M> const & other) {
		int counts = _counts <= other._counts ? _counts : other._counts;
		for (int i = 0; i < counts; i++) {
			_datas[i] = static_cast<T>(other._datas[i]);
		}
	}

	template <typename T, unsigned int N>
	template<unsigned int M>//同类型不同实数赋值
	LhVertex<T, N>::LhVertex<T, N>& operator=(LhVertex<T, M> const & other) {
		int counts = _counts <= other._counts ? _counts : other._counts;
		for (int i = 0; i < counts; i++) {
			_datas[i] = other._datas[i];
		}
	}

	template <typename T, unsigned int N>
	template<typename U>//不同类型同实数赋值
	LhVertex<T, N>::LhVertex<T, N>& operator=(LhVertex<U, N> const & other) {
		for (int i = 0; i < _counts; i++) {
			_datas[i] = static_cast<T>(other._datas[i]);
		}
	}

	template <typename T, unsigned int N>
	template<typename U, unsigned int M>//不同类型不同实数赋值
	LhVertex<T, N>::LhVertex<T, N>& operator=(LhVertex<U, M> const & other) {
		int counts = _counts <= other._counts ? _counts : other._counts;
		for (int i = 0; i < counts; i++) {
			_datas[i] = static_cast<T>(other._datas[i]);
		}
	}
*/
//}
