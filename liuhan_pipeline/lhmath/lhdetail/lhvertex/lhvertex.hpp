#pragma once

//namespace lh_pipeline {
    template <typename T, unsigned int N>
    class LhVertex {
    public:
        ~LhVertex() {}
        LhVertex();
        LhVertex(LhVertex<T, N> const & other);
        LhVertex<T, N>& operator=(LhVertex<T, N> const & other);
/*
//不同类型或不同实数对象间的拷贝与赋值
        template<unsigned int M>
        LhVertex(LhVertex<T, M> const & other);
        template<typename U>
        LhVertex(LhVertex<U, N> const & other);
        template<typename U, unsigned int M>
        LhVertex(LhVertex<U, M> const & other);

        template<unsigned int M>
        LhVertex<T, N>& operator=(LhVertex<T, M> const & other);
        template<typename U>
        LhVertex<T, N>& operator=(LhVertex<U, N> const & other);
        template<typename U, unsigned int M>
        LhVertex<T, N>& operator=(LhVertex<U, M> const & other);
*/

    public:
/*
        T dot(LhVertex<T, N> const & other);//点乘
        bool normalize();//单位化
*/
    protected:
        unsigned int _counts = N;
        T _datas[N];
    };

#ifndef EXTENDED_IMPLEMENTATION_LHVERTEX//实现
#include "lhvertex.inl"
#endif

#ifndef EXTENDED_IMPLEMENTATION_BINARY_LHVERTEX//一些二元操作
    template <typename T, unsigned int N>
    LhVertex<T, N>& operator+(LhVertex<T, N> const & left, LhVertex<T, N> const& right);//加
    template <typename T, unsigned int N>
    LhVertex<T, N>& operator-(LhVertex<T, N> const & left, LhVertex<T, N> const& right);//减
    template <typename T, unsigned int N>
    LhVertex<T, N>& operator*(LhVertex<T, N> const & left, LhVertex<T, N> const& right);//乘
    template <typename T, unsigned int N>
    LhVertex<T, N>& operator/(LhVertex<T, N> const & left, LhVertex<T, N> const& right);//除

    template <typename T, unsigned int N>
    LhVertex<T, N>& operator+(LhVertex<T, N> const & left, const T& scalar);//加
    template <typename T, unsigned int N>
    LhVertex<T, N>& operator-(LhVertex<T, N> const & left, const T& scalar);//减
    template <typename T, unsigned int N>
    LhVertex<T, N>& operator*(LhVertex<T, N> const & left, const T& scalar);//乘
    template <typename T, unsigned int N>
    LhVertex<T, N>& operator/(LhVertex<T, N> const & left, const T& scalar);//除

    template <typename T, unsigned int N>
    bool operator==(LhVertex<T, N> const & left, LhVertex<T, N> const& right);//==

    template <typename T, unsigned int N>
    bool operator!=(LhVertex<T, N> const & left, LhVertex<T, N> const& right);//!=


#include "lhvertex_binary.hpp"
#endif

#ifndef EXTENDED_IMPLEMENTATION_SPECIALIZATION_LHVERTEX//特化float和int类型的成员构造函数
#include "lhvertex_specialization.hpp"
    
    SpecializationInt(2)//LhVertex<int, 2>
    SpecializationInt(3)//LhVertex<int, 3>
    SpecializationInt(4)//LhVertex<int, 4>

    SpecializationFloat(2)//LhVertex<float, 2>
    SpecializationFloat(3)//LhVertex<float, 3>
    SpecializationFloat(4)//LhVertex<float, 4>

#endif
//}