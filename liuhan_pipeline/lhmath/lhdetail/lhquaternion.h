#pragma once
#include "lhvertex.h"

namespace lh_pipeline {
    class LhQuaternion
    {
    public:
        LhQuaternion();
        ~LhQuaternion();
    public:
        LhQuaternion(float _x, float _y, float _z, float _w);
        void normalize();
        LhQuaternion conjugate();
        LhVertexFloat3 todegrees();

    public:
        float _x;
        float _y;
        float _z;
        float _w;
    };

    LhQuaternion operator*(const LhQuaternion& l, const LhQuaternion& r);
    LhQuaternion operator*(const LhQuaternion& q, const LhVertexFloat3& v);
}

