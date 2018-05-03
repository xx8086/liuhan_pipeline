#include "lhquaternion.h"
#include "..\lhmath_common.h"

LhQuaternion::LhQuaternion()
{
}


LhQuaternion::~LhQuaternion()
{
}


LhQuaternion::LhQuaternion(float _x, float _y, float _z, float _w){
    x = _x;
    y = _y;
    z = _z;
    w = _w;
}

void LhQuaternion::normalize(){
    float Length = sqrtf(x * x + y * y + z * z + w * w);
    x /= Length;
    y /= Length;
    z /= Length;
    w /= Length;
}


LhQuaternion LhQuaternion::conjugate(){
    return LhQuaternion(-x, -y, -z, w);
}

LhQuaternion operator*(const LhQuaternion& l, const LhQuaternion& r)
{
    const float w = (l.w * r.w) - (l.x * r.x) - (l.y * r.y) - (l.z * r.z);
    const float x = (l.x * r.w) + (l.w * r.x) + (l.y * r.z) - (l.z * r.y);
    const float y = (l.y * r.w) + (l.w * r.y) + (l.z * r.x) - (l.x * r.z);
    const float z = (l.z * r.w) + (l.w * r.z) + (l.x * r.y) - (l.y * r.x);
    return LhQuaternion(x, y, z, w);
}

LhQuaternion operator*(const LhQuaternion& q, const LhVertex<float, 3>& v)
{
    const float w = -(q.x * v.get_x()) - (q.y * v.get_y()) - (q.z * v.get_z());
    const float x = (q.w * v.get_x()) + (q.y * v.get_z()) - (q.z * v.get_y());
    const float y = (q.w * v.get_y()) + (q.z * v.get_x()) - (q.x * v.get_z());
    const float z = (q.w * v.get_z()) + (q.x * v.get_y()) - (q.y * v.get_x());
    return LhQuaternion(x, y, z, w);
}


LhVertex<float, 3> LhQuaternion::todegrees()
{
    float f[3];
    f[0] = atan2(x * z + y * w, x * w - y * z);
    f[1] = acos(-x * x - y * y - z * z - w * w);
    f[2] = atan2(x * z - y * w, x * w + y * z);
    return LhVertex<float, 3>(TDegreeF(f[0]), TDegreeF(f[1]), TDegreeF(f[2]));
}

LhVertex<float, 3> LhQuaternion::rotate(float angle, 
    const LhVertex<float, 3>& axe,
    const LhVertex<float, 3>& view) {
    const float sin_half_angle = sinf(ToRadianF(angle / 2.0f));
    const float cos_half_angle = cosf(ToRadianF(angle / 2.0f));
    const float rx = axe.get_x() * sin_half_angle;
    const float ry = axe.get_y() * sin_half_angle;
    const float rz = axe.get_z() * sin_half_angle;
    const float rw = cos_half_angle;
    LhQuaternion rotation(rx, ry, rz, rw);
    LhQuaternion conjugate = rotation.conjugate();
    LhQuaternion w = rotation * view * conjugate;
    return LhVertex<float, 3>(w.x, w.y, w.z);
}
