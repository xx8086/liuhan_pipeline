#pragma once
#include "lhvertex\lhvertex.hpp"

class LhQuaternion{
public:
    LhQuaternion();
    ~LhQuaternion();
    LhQuaternion(float _x, float _y, float _z, float _w);
    void normalize();
    LhQuaternion conjugate();
    LhVertex<float, 3> todegrees();
    LhVertex<float, 3> rotate(float angle, const LhVertex<float, 3>& axe, const LhVertex<float, 3>& view);

    float x, y, z, w;
};

LhQuaternion operator*(const LhQuaternion& q, const LhVertex<float, 3>& v);
LhQuaternion operator*(const LhQuaternion& l, const LhQuaternion& r);

