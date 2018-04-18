#pragma once
#include "lhvertex\lhvertex.hpp"

namespace lh_pipeline {

    typedef LhVertex<float, 2> LhVertexFloat2;
    typedef LhVertex<float, 3> LhVertexFloat3;
    typedef LhVertex<float, 4> LhVertexFloat4;

    typedef LhVertex<int, 2> LhVertexInt2;
    typedef LhVertex<int, 3> LhVertexInt3;
    typedef LhVertex<int, 4> LhVertexInt4;

    
    //单位化
    bool normalize(LhVertex<float, 3>& vf3);

    //点乘
    float dot(const LhVertex<float, 3>& left, const LhVertex<float, 3>& right);

    // 叉乘
    static LhVertex<float, 3> cross(const LhVertex<float, 3>& left, const LhVertex<float, 3>& right) {
        return LhVertex<float, 3>(
            left.get_y() * right.get_z() - left.get_z() * right.get_y(),
            left.get_z() * right.get_x() - left.get_x() * right.get_z(),
            left.get_x() * right.get_y() - left.get_y() * right.get_x());
    }
}