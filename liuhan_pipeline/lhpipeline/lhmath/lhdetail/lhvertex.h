#pragma once
#include "lhvertex\lhvertex.hpp"
#define MAX(a, b) (a > b ? a : b) 
#define MIN(a, b) (a > b ? b : a)

namespace lh_pipeline {

    typedef LhVertex<float, 2> LhVertexFloat2;
    typedef LhVertex<float, 3> LhVertexFloat3;
    typedef LhVertex<float, 4> LhVertexFloat4;

    typedef LhVertex<int, 2> LhVertexInt2;
    typedef LhVertex<int, 3> LhVertexInt3;
    typedef LhVertex<int, 4> LhVertexInt4;

    
    //单位化
    bool normalize(LhVertexFloat3& vf3);

    //点乘
    static float dot(const LhVertexFloat3& left, const LhVertexFloat3& right) {
        return left.get_x() * right.get_x() +
            left.get_y() * right.get_y() +
            left.get_z() * right.get_z();
    }

	//反射
	static LhVertexFloat3 reflect(LhVertexFloat3 const & I, LhVertexFloat3 const & N){
		return I - N * dot(N, I) * 2.0f;
	}

    // 叉乘
    static LhVertexFloat3 cross(const LhVertexFloat3& left, const LhVertexFloat3& right) {
        return LhVertexFloat3(
            left.get_y() * right.get_z() - left.get_z() * right.get_y(),
            left.get_z() * right.get_x() - left.get_x() * right.get_z(),
            left.get_x() * right.get_y() - left.get_y() * right.get_x());
    }

	static LhVertexFloat3 get_normal(const LhVertexFloat3& v1, const LhVertexFloat3& v2, const LhVertexFloat3& v3) {
		LhVertexFloat3 n1 = v2 - v1;
		LhVertexFloat3 n2 = v3 - v1;
		LhVertexFloat3 n = cross(n1, n2);
		normalize(n);
		return n;
	}

	template <typename genType>
	 genType lh_clamp(genType x, genType minVal, genType maxVal){
		return MIN(MAX(x, minVal), maxVal);
	}


}