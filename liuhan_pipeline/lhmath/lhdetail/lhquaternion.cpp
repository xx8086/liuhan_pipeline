#include "lhQuaternion.h"
#include <math.h>
#include "../lhmath_common.h"

namespace lh_pipeline {
    LhQuaternion::LhQuaternion()
    {
    }


    LhQuaternion::~LhQuaternion()
    {
    }

    LhQuaternion::LhQuaternion(float x, float y, float z, float w)
        :_x(x), _y(y), _z(z), _w(w) {}

    void LhQuaternion::normalize() {
        float length = sqrtf(_x * _x + _y * _y + _z * _z + _w * _w);
        _x /= length;
        _y /= length;
        _z /= length;
        _w /= length;
    }
    LhQuaternion LhQuaternion::conjugate() {
        return LhQuaternion(-_x, -_y, -_z, _w);
    }

    LhVertexFloat3 LhQuaternion::todegrees() {
        float f[3];

        f[0] = atan2(_x * _z + _y * _w, _x * _w - _y * _z);
        f[1] = acos(-_x * _x - _y * _y - _z * _z - _w * _w);
        f[2] = atan2(_x * _z - _y * _w, _x * _w + _y * _z);

        f[0] = TDegreeF(f[0]);
        f[1] = TDegreeF(f[1]);
        f[2] = TDegreeF(f[2]);
        return LhVertexFloat3(f[0], f[1], f[2]);
    }

    void LhQuaternion::rotate(LhVertex<float, 3>& vf3,
        const LhVertex<float, 3>& axis,
        const float angle) {//vf3ÈÆaxisÖáÐý×ªangle¶È
        const float sin_half_angle = sinf(ToRadianF(angle / 2.0f));
        const float cos_half_angle = cosf(ToRadianF(angle / 2.0f));

        const float rx = axis.get_x() * sin_half_angle;
        const float ry = axis.get_y() * sin_half_angle;
        const float rz = axis.get_z() * sin_half_angle;
        const float rw = cos_half_angle;
        LhQuaternion rotateq(rx, ry, rz, rw);
        LhQuaternion rotatew = rotateq * vf3 * rotateq.conjugate();
        vf3.set_x(rotatew._x);
        vf3.set_y(rotatew._y);
        vf3.set_z(rotatew._z);
    }

    LhQuaternion operator*(const LhQuaternion& l, const LhQuaternion& r)
    {
        const float w = (l._w * r._w) - (l._x * r._x) - (l._y * r._y) - (l._z * r._z);
        const float x = (l._x * r._w) + (l._w * r._x) + (l._y * r._z) - (l._z * r._y);
        const float y = (l._y * r._w) + (l._w * r._y) + (l._z * r._x) - (l._x * r._z);
        const float z = (l._z * r._w) + (l._w * r._z) + (l._x * r._y) - (l._y * r._x);
        return LhQuaternion(x, y, z, w);
    }

    LhQuaternion operator*(const LhQuaternion& q, const LhVertexFloat3& v)
    {
        const float w = -(q._x * v.get_x()) - (q._y * v.get_y()) - (q._z * v.get_z());
        const float x = (q._w * v.get_x()) + (q._y * v.get_z()) - (q._z * v.get_y());
        const float y = (q._w * v.get_y()) + (q._z * v.get_x()) - (q._x * v.get_z());
        const float z = (q._w * v.get_z()) + (q._x * v.get_y()) - (q._y * v.get_x());
        return LhQuaternion(x, y, z, w);
    }
}