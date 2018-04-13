#pragma once
#include "lhvertex\lhvertex.hpp"
#include"..\lhmath_common.h"

namespace lh_pipeline {

    typedef LhVertex<float, 2> LhVertexFloat2;
    typedef LhVertex<float, 3> LhVertexFloat3;
    typedef LhVertex<float, 4> LhVertexFloat4;

    typedef LhVertex<int, 2> LhVertexInt2;
    typedef LhVertex<int, 3> LhVertexInt3;
    typedef LhVertex<int, 4> LhVertexInt4;

/*
    bool normalize(LhVertex<float, 3>& vf3) {
        const float length = sqrtf(vf3.get_x() * vf3.get_x() + vf3.get_y() * vf3.get_y() + vf3.get_z() * vf3.get_z());
        if (length > 0.00001) {
            vf3.set_x(vf3.get_x() / length);
            vf3.set_y(vf3.get_y() / length);
            vf3.set_z(vf3.get_z() / length);
        }
        else {
            return false;
        }
        return true;
    }
*/
    //²æ³Ë
    inline LhVertex<float, 3> cross(const LhVertex<float, 3>& left, const LhVertex<float, 3>& right) {
        return LhVertex<float, 3>(
            left.get_y() * right.get_z() - left.get_z() * right.get_y(), 
            left.get_z() * right.get_x() - left.get_x() * right.get_z(), 
            left.get_x() * right.get_y() - left.get_y() * right.get_x());
    }

}