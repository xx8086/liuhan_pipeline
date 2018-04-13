#pragma once
#include "lhvertex\lhvertex.hpp"

namespace lh_pipeline {

    typedef LhVertex<float, 2> LhVertexFloat2;
    typedef LhVertex<float, 3> LhVertexFloat3;
    typedef LhVertex<float, 4> LhVertexFloat4;

    typedef LhVertex<int, 2> LhVertexInt2;
    typedef LhVertex<int, 3> LhVertexInt3;
    typedef LhVertex<int, 4> LhVertexInt4;

    //��λ��
    inline bool normalize(LhVertex<float, 3>& vf3);

    //���
    inline float dot(const LhVertex<float, 3>& left, const LhVertex<float, 3>& right);

    //���
    inline LhVertex<float, 3> cross(const LhVertex<float, 3>& left, const LhVertex<float, 3>& right);

}