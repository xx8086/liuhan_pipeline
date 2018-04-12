#include "lhvertex.hpp"
#pragma once

namespace lh_pipeline {
	template <typename T, Precision P>
	LhVertex3<T, P>::~LhVertex3() {}

	template <typename T, Precision P>
	constexpr LhVertex3<T, P>::LhVertex3()
		: x(0), y(0), z(0){}

	template <typename T, Precision P>
	constexpr LhVertex3<T, P>::LhVertex3(T scalar)
		: x(scalar), y(scalar), z(scalar)
	{}

	template <typename T, Precision P>
	constexpr  LhVertex3<T, P>::LhVertex3(T a, T b, T c)
		: x(a), y(b), z(c)
	{}

	template <typename T, Precision P>
	template <typename A, typename B, typename C>
	constexpr LhVertex3<T, P>::LhVertex3(A a, B b, C c) :
		x(static_cast<T>(a)),
		y(static_cast<T>(b)),
		z(static_cast<T>(c))
	{}

	template<typename T, Precision P>
	template<Precision Q>
	inline constexpr LhVertex3<T, P>::LhVertex3(tvec3<T, Q> const & v)
	{
	}

	template <typename T, Precision P>
	LhVertex3<T, P>& LhVertex3<T, P>::operator=(LhVertex3<T, P> const & v)
	{
		this->x = v.x;
		this->y = v.y;
		this->z = v.z;
		return *this;
	}

	template <typename T, Precision P>
	template <typename U>
	LhVertex3<T, P>& LhVertex3<T, P>::operator=(LhVertex3<U, P> const & v)
	{
		this->x = static_cast<T>(v.x);
		this->y = static_cast<T>(v.y);
		this->z = static_cast<T>(v.z);
		return *this;
	}

	//·ûºÅ
	template <typename T, Precision P>
	LhVertex3<T, P> operator+(LhVertex3<T, P> const & v) {
		return v;
	}

	template <typename T, Precision P>
	LhVertex3<T, P> operator-(LhVertex3<T, P> const & v) {
		return LhVertex3<T, P>(-v.x, -v.y, -v.z);
	}
	

	//¼Ó¼õ³Ë³ý
	template <typename T, Precision P>
	LhVertex3<T, P> operator+(LhVertex3<T, P> const & v1, LhVertex3<T, P> const & v2)
	{
		return LhVertex3<T, P>(
			v1.x + v2.x,
			v1.y + v2.y,
			v1.z + v2.z);
	}

	template <typename T, Precision P>
	LhVertex3<T, P> operator-(LhVertex3<T, P> const & v1, LhVertex3<T, P> const & v2)
	{
		return LhVertex3<T, P>(
			v1.x - v2.x,
			v1.y - v2.y,
			v1.z - v2.z);
	}

	template <typename T, Precision P>
	LhVertex3<T, P> operator*(LhVertex3<T, P> const & v1, LhVertex3<T, P> const & v2)
	{
		return LhVertex3<T, P>(
			v1.x * v2.x,
			v1.y * v2.y,
			v1.z * v2.z);
	}

	template <typename T, Precision P>
	LhVertex3<T, P> operator/(LhVertex3<T, P> const & v1, LhVertex3<T, P> const & v2)
	{
		return LhVertex3<T, P>(
			v1.x / v2.x,
			v1.y / v2.y,
			v1.z / v2.z);
	}

	template <typename T, Precision P>
	LhVertex3<T, P> operator+(LhVertex3<T, P> const & v1, T scalar) {
		return LhVertex3<T, P>(
			v1.x + scalar,
			v1.y + scalar,
			v1.z + scalar);
	}
	template <typename T, Precision P>
	LhVertex3<T, P> operator-(LhVertex3<T, P> const & v1, T scalar) {
		return LhVertex3<T, P>(
			v1.x - scalar,
			v1.y - scalar,
			v1.z - scalar);
	}
	template <typename T, Precision P>
	LhVertex3<T, P> operator*(LhVertex3<T, P> const & v1, T scalar) {
		return LhVertex3<T, P>(
			v1.x * scalar,
			v1.y * scalar,
			v1.z * scalar);
	}
	template <typename T, Precision P>
	LhVertex3<T, P> operator/(LhVertex3<T, P> const & v1, T scalar) {
		return LhVertex3<T, P>(
			v1.x / scalar,
			v1.y / scalar,
			v1.z / scalar);
	}

	//
	template <typename T, Precision P>
	bool operator==(LhVertex3<T, P> const & v1, LhVertex3<T, P> const & v2) {
		return (v1.x == v2.x) && (v1.y == v2.y) && (v1.z == v2.z);
	}

	template <typename T, Precision P>
	bool operator!=(LhVertex3<T, P> const & v1, LhVertex3<T, P> const & v2) {
		return (v1.x != v2.x) || (v1.y != v2.y) || (v1.z != v2.z);
	}

	template <Precision P>
	LhVertex3<bool, P> operator&&(LhVertex3<bool, P> const & v1, LhVertex3<bool, P> const & v2) {
		return LhVertex3<bool, P>(v1.x && v2.x, v1.y && v2.y, v1.z && v2.z);
	}

	template <Precision P>
	LhVertex3<bool, P> operator||(LhVertex3<bool, P> const & v1, LhVertex3<bool, P> const & v2) {
		return LhVertex3<bool, P>(v1.x || v2.x, v1.y || v2.y, v1.z || v2.z);
	}

}