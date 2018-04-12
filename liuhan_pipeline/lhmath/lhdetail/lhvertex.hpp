#pragma once
#include <cstddef>

namespace lh_pipeline {
	enum Precision
	{
		packed_highp,
		packed_mediump,
		packed_lowp,

		highp = packed_highp,
		mediump = packed_mediump,
		lowp = packed_lowp,
		packed = packed_highp,

		defaultp = highp
	};

	template <typename T, Precision P = defaultp>
	class LhVertex3
	{
	public:
		typedef T value_type;
		typedef LhVertex3<T, P> type;
		typedef LhVertex3<bool, P> bool_type;

		union{
			struct { T x, y, z; };
			struct { T r, g, b; };
			struct { T s, t, p; };
		};
	public:
		~LhVertex3();
		constexpr LhVertex3();
		constexpr LhVertex3(T a, T b, T c);
		constexpr LhVertex3(T scalar);
		template <typename A, typename B, typename C>
		constexpr LhVertex3(A a, B b, C c);
		template <Precision Q>
		constexpr LhVertex3(LhVerTex<T, Q> const & v);
	public:
		LhVertex3<T, P> & operator=(LhVertex3<T, P> const & v);

		template <typename U>
		LhVertex3<T, P> & operator=(LhVertex3<U, P> const & v);//=
	};

	//operators
	template <typename T, Precision P>
	LhVertex3<T, P> operator+(LhVertex3<T, P> const & v);//ÕýºÅ

	template <typename T, Precision P>
	LhVertex3<T, P> operator-(LhVertex3<T, P> const & v);//¸ººÅ

	template <typename T, Precision P>
	LhVertex3<T, P> operator+(LhVertex3<T, P> const & v1, LhVertex3<T, P> const & v2);//¼Ó
	template <typename T, Precision P>
	LhVertex3<T, P> operator-(LhVertex3<T, P> const & v1, LhVertex3<T, P> const & v2);//¼õ
	template <typename T, Precision P>
	LhVertex3<T, P> operator*(LhVertex3<T, P> const & v1, LhVertex3<T, P> const & v2);//³Ë
	template <typename T, Precision P>
	LhVertex3<T, P> operator/(LhVertex3<T, P> const & v1, LhVertex3<T, P> const & v2);//³ý

	template <typename T, Precision P>
	LhVertex3<T, P> operator+(LhVertex3<T, P> const & v1, T scalar);//¼Ó
	template <typename T, Precision P>
	LhVertex3<T, P> operator-(LhVertex3<T, P> const & v1, T scalar);//¼õ
	template <typename T, Precision P>
	LhVertex3<T, P> operator*(LhVertex3<T, P> const & v1, T scalar);//³Ë
	template <typename T, Precision P>
	LhVertex3<T, P> operator/(LhVertex3<T, P> const & v1, T scalar);//³ý

	template <typename T, Precision P>
	bool operator==(LhVertex3<T, P> const & v1, LhVertex3<T, P> const & v2);//==

	template <typename T, Precision P>
	bool operator!=(LhVertex3<T, P> const & v1, LhVertex3<T, P> const & v2);//!=

	template <Precision P>
	LhVertex3<bool, P> operator&&(LhVertex3<bool, P> const & v1, LhVertex3<bool, P> const & v2);//&&

	template <Precision P>
	LhVertex3<bool, P> operator||(LhVertex3<bool, P> const & v1, LhVertex3<bool, P> const & v2);//||
}

#ifndef LH_MATH_EXTERNAL_TEMPLATE
#include "lhvertex.inl"
#endif