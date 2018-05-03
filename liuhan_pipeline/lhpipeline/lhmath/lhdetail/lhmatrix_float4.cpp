#include "lhmatrix_float4.h"
#include <memory.h>
#include <assert.h>
#include <cmath>
#include"..\lhmath_common.h"
#include "lhvertex.h"

namespace lh_pipeline {
    LhMatrixFloat4::LhMatrixFloat4() {
    }


    LhMatrixFloat4::~LhMatrixFloat4() {
    }

    LhMatrixFloat4::LhMatrixFloat4(
        float a00, float a01, float a02, float a03,
        float a10, float a11, float a12, float a13,
        float a20, float a21, float a22, float a23,
        float a30, float a31, float a32, float a33) {
        _m[0][0] = a00; _m[0][1] = a01; _m[0][2] = a02; _m[0][3] = a03;
        _m[1][0] = a10; _m[1][1] = a11; _m[1][2] = a12; _m[1][3] = a13;
        _m[2][0] = a20; _m[2][1] = a21; _m[2][2] = a22; _m[2][3] = a23;
        _m[3][0] = a30; _m[3][1] = a31; _m[3][2] = a32; _m[3][3] = a33;
    }

    void LhMatrixFloat4::set_zero() {
        memset(_m, 0, sizeof(_m));
    }

    LhMatrixFloat4 LhMatrixFloat4::transpose() {
        LhMatrixFloat4 n;
        for (unsigned int i = 0; i < 4; i++) {
            for (unsigned int j = 0; j < 4; j++) {
                n._m[i][j] = _m[j][i];
            }
        }

        return n;
    }

    void LhMatrixFloat4::identity() {
        _m[0][0] = 1.0f; _m[0][1] = 0.0f; _m[0][2] = 0.0f; _m[0][3] = 0.0f;
        _m[1][0] = 0.0f; _m[1][1] = 1.0f; _m[1][2] = 0.0f; _m[1][3] = 0.0f;
        _m[2][0] = 0.0f; _m[2][1] = 0.0f; _m[2][2] = 1.0f; _m[2][3] = 0.0f;
        _m[3][0] = 0.0f; _m[3][1] = 0.0f; _m[3][2] = 0.0f; _m[3][3] = 1.0f;
    }

    LhVertexFloat4 LhMatrixFloat4::operator*(const LhVertexFloat3& right) {
        LhVertexFloat4 r;
        r.set_x(_m[0][0] * right.get_x() + _m[0][1] * right.get_y() + _m[0][2] * right.get_z() + _m[0][3]);
        r.set_y(_m[1][0] * right.get_x() + _m[1][1] * right.get_y() + _m[1][2] * right.get_z() + _m[1][3]);
        r.set_z(_m[2][0] * right.get_x() + _m[2][1] * right.get_y() + _m[2][2] * right.get_z() + _m[2][3]);
        r.set_w(_m[3][0] * right.get_x() + _m[3][1] * right.get_y() + _m[3][2] * right.get_z() + _m[3][3]);
        return r;
    }

    LhVertexFloat4 LhMatrixFloat4::operator*(const LhVertexFloat4& right) {
        LhVertexFloat4 r;
        r.set_x(_m[0][0] * right.get_x() + _m[0][1] * right.get_y() + _m[0][2] * right.get_z() + _m[0][3] * right.get_w());
        r.set_y(_m[1][0] * right.get_x() + _m[1][1] * right.get_y() + _m[1][2] * right.get_z() + _m[1][3] * right.get_w());
        r.set_z(_m[2][0] * right.get_x() + _m[2][1] * right.get_y() + _m[2][2] * right.get_z() + _m[2][3] * right.get_w());
        r.set_w(_m[3][0] * right.get_x() + _m[3][1] * right.get_y() + _m[3][2] * right.get_z() + _m[3][3] * right.get_w());
        return r;
    }

    LhMatrixFloat4 LhMatrixFloat4::operator*(const LhMatrixFloat4& right) 
    {
        LhMatrixFloat4 ret;
        for (unsigned int i = 0; i < 4; i++)
        {
            for (unsigned int j = 0; j < 4; j++)
            {
                ret._m[i][j] =
                    _m[i][0] * right._m[0][j] +
                    _m[i][1] * right._m[1][j] +
                    _m[i][2] * right._m[2][j] +
                    _m[i][3] * right._m[3][j];
            }
        }
        return ret;
    }

    float LhMatrixFloat4::determinant() const {//行列式
        return _m[0][0] * _m[1][1] * _m[2][2] * _m[3][3] - _m[0][0] * _m[1][1] * _m[2][3] * _m[3][2] + _m[0][0] * _m[1][2] * _m[2][3] * _m[3][1] - _m[0][0] * _m[1][2] * _m[2][1] * _m[3][3]
            + _m[0][0] * _m[1][3] * _m[2][1] * _m[3][2] - _m[0][0] * _m[1][3] * _m[2][2] * _m[3][1] - _m[0][1] * _m[1][2] * _m[2][3] * _m[3][0] + _m[0][1] * _m[1][2] * _m[2][0] * _m[3][3]
            - _m[0][1] * _m[1][3] * _m[2][0] * _m[3][2] + _m[0][1] * _m[1][3] * _m[2][2] * _m[3][0] - _m[0][1] * _m[1][0] * _m[2][2] * _m[3][3] + _m[0][1] * _m[1][0] * _m[2][3] * _m[3][2]
            + _m[0][2] * _m[1][3] * _m[2][0] * _m[3][1] - _m[0][2] * _m[1][3] * _m[2][1] * _m[3][0] + _m[0][2] * _m[1][0] * _m[2][1] * _m[3][3] - _m[0][2] * _m[1][0] * _m[2][3] * _m[3][1]
            + _m[0][2] * _m[1][1] * _m[2][3] * _m[3][0] - _m[0][2] * _m[1][1] * _m[2][0] * _m[3][3] - _m[0][3] * _m[1][0] * _m[2][1] * _m[3][2] + _m[0][3] * _m[1][0] * _m[2][2] * _m[3][1]
            - _m[0][3] * _m[1][1] * _m[2][2] * _m[3][0] + _m[0][3] * _m[1][1] * _m[2][0] * _m[3][2] - _m[0][3] * _m[1][2] * _m[2][0] * _m[3][1] + _m[0][3] * _m[1][2] * _m[2][1] * _m[3][0];
    }

    LhMatrixFloat4& LhMatrixFloat4::inverse() {//逆矩阵
        float det = determinant();
        if (det == 0.0f) {
            assert(0);
            return *this;
        }

        float invdet = 1.0f / det;
        LhMatrixFloat4 res;
        res._m[0][0] = invdet * (_m[1][1] * (_m[2][2] * _m[3][3] - _m[2][3] * _m[3][2]) + _m[1][2] * (_m[2][3] * _m[3][1] - _m[2][1] * _m[3][3]) + _m[1][3] * (_m[2][1] * _m[3][2] - _m[2][2] * _m[3][1]));
        res._m[0][1] = -invdet * (_m[0][1] * (_m[2][2] * _m[3][3] - _m[2][3] * _m[3][2]) + _m[0][2] * (_m[2][3] * _m[3][1] - _m[2][1] * _m[3][3]) + _m[0][3] * (_m[2][1] * _m[3][2] - _m[2][2] * _m[3][1]));
        res._m[0][2] = invdet * (_m[0][1] * (_m[1][2] * _m[3][3] - _m[1][3] * _m[3][2]) + _m[0][2] * (_m[1][3] * _m[3][1] - _m[1][1] * _m[3][3]) + _m[0][3] * (_m[1][1] * _m[3][2] - _m[1][2] * _m[3][1]));
        res._m[0][3] = -invdet * (_m[0][1] * (_m[1][2] * _m[2][3] - _m[1][3] * _m[2][2]) + _m[0][2] * (_m[1][3] * _m[2][1] - _m[1][1] * _m[2][3]) + _m[0][3] * (_m[1][1] * _m[2][2] - _m[1][2] * _m[2][1]));
        res._m[1][0] = -invdet * (_m[1][0] * (_m[2][2] * _m[3][3] - _m[2][3] * _m[3][2]) + _m[1][2] * (_m[2][3] * _m[3][0] - _m[2][0] * _m[3][3]) + _m[1][3] * (_m[2][0] * _m[3][2] - _m[2][2] * _m[3][0]));
        res._m[1][1] = invdet * (_m[0][0] * (_m[2][2] * _m[3][3] - _m[2][3] * _m[3][2]) + _m[0][2] * (_m[2][3] * _m[3][0] - _m[2][0] * _m[3][3]) + _m[0][3] * (_m[2][0] * _m[3][2] - _m[2][2] * _m[3][0]));
        res._m[1][2] = -invdet * (_m[0][0] * (_m[1][2] * _m[3][3] - _m[1][3] * _m[3][2]) + _m[0][2] * (_m[1][3] * _m[3][0] - _m[1][0] * _m[3][3]) + _m[0][3] * (_m[1][0] * _m[3][2] - _m[1][2] * _m[3][0]));
        res._m[1][3] = invdet * (_m[0][0] * (_m[1][2] * _m[2][3] - _m[1][3] * _m[2][2]) + _m[0][2] * (_m[1][3] * _m[2][0] - _m[1][0] * _m[2][3]) + _m[0][3] * (_m[1][0] * _m[2][2] - _m[1][2] * _m[2][0]));
        res._m[2][0] = invdet * (_m[1][0] * (_m[2][1] * _m[3][3] - _m[2][3] * _m[3][1]) + _m[1][1] * (_m[2][3] * _m[3][0] - _m[2][0] * _m[3][3]) + _m[1][3] * (_m[2][0] * _m[3][1] - _m[2][1] * _m[3][0]));
        res._m[2][1] = -invdet * (_m[0][0] * (_m[2][1] * _m[3][3] - _m[2][3] * _m[3][1]) + _m[0][1] * (_m[2][3] * _m[3][0] - _m[2][0] * _m[3][3]) + _m[0][3] * (_m[2][0] * _m[3][1] - _m[2][1] * _m[3][0]));
        res._m[2][2] = invdet * (_m[0][0] * (_m[1][1] * _m[3][3] - _m[1][3] * _m[3][1]) + _m[0][1] * (_m[1][3] * _m[3][0] - _m[1][0] * _m[3][3]) + _m[0][3] * (_m[1][0] * _m[3][1] - _m[1][1] * _m[3][0]));
        res._m[2][3] = -invdet * (_m[0][0] * (_m[1][1] * _m[2][3] - _m[1][3] * _m[2][1]) + _m[0][1] * (_m[1][3] * _m[2][0] - _m[1][0] * _m[2][3]) + _m[0][3] * (_m[1][0] * _m[2][1] - _m[1][1] * _m[2][0]));
        res._m[3][0] = -invdet * (_m[1][0] * (_m[2][1] * _m[3][2] - _m[2][2] * _m[3][1]) + _m[1][1] * (_m[2][2] * _m[3][0] - _m[2][0] * _m[3][2]) + _m[1][2] * (_m[2][0] * _m[3][1] - _m[2][1] * _m[3][0]));
        res._m[3][1] = invdet * (_m[0][0] * (_m[2][1] * _m[3][2] - _m[2][2] * _m[3][1]) + _m[0][1] * (_m[2][2] * _m[3][0] - _m[2][0] * _m[3][2]) + _m[0][2] * (_m[2][0] * _m[3][1] - _m[2][1] * _m[3][0]));
        res._m[3][2] = -invdet * (_m[0][0] * (_m[1][1] * _m[3][2] - _m[1][2] * _m[3][1]) + _m[0][1] * (_m[1][2] * _m[3][0] - _m[1][0] * _m[3][2]) + _m[0][2] * (_m[1][0] * _m[3][1] - _m[1][1] * _m[3][0]));
        res._m[3][3] = invdet * (_m[0][0] * (_m[1][1] * _m[2][2] - _m[1][2] * _m[2][1]) + _m[0][1] * (_m[1][2] * _m[2][0] - _m[1][0] * _m[2][2]) + _m[0][2] * (_m[1][0] * _m[2][1] - _m[1][1] * _m[2][0]));
        *this = res;
        return *this;
    }

    void LhMatrixFloat4::scale_transform(float x, float y, float z) {
        _m[0][0] = x;       _m[0][1] = 0.0f;    _m[0][2] = 0.0f;    _m[0][3] = 0.0f;
        _m[1][0] = 0.0f;    _m[1][1] = y;       _m[1][2] = 0.0f;    _m[1][3] = 0.0f;
        _m[2][0] = 0.0f;    _m[2][1] = 0.0f;    _m[2][2] = z;       _m[2][3] = 0.0f;
        _m[3][0] = 0.0f;    _m[3][1] = 0.0f;    _m[3][2] = 0.0f;    _m[3][3] = 1.0f;
    }

    void LhMatrixFloat4::rotate_transform(float rotate_x, float rotate_y, float rotate_z) {
        LhMatrixFloat4 rx;
        LhMatrixFloat4 ry;
        LhMatrixFloat4 rz;
        const float x = ToRadianF(rotate_x);
        const float y = ToRadianF(rotate_y);
        const float z = ToRadianF(rotate_z);

        //绕X轴旋转x度
        rx._m[0][0] = 1.0f; rx._m[0][1] = 0.0f;     rx._m[0][2] = 0.0f;     rx._m[0][3] = 0.0f;
        rx._m[1][0] = 0.0f; rx._m[1][1] = cosf(x);  rx._m[1][2] = -sinf(x); rx._m[1][3] = 0.0f;
        rx._m[2][0] = 0.0f; rx._m[2][1] = sinf(x);  rx._m[2][2] = cosf(x);  rx._m[2][3] = 0.0f;
        rx._m[3][0] = 0.0f; rx._m[3][1] = 0.0f;     rx._m[3][2] = 0.0f;     rx._m[3][3] = 1.0f;

        //绕Y轴旋转y度
        ry._m[0][0] = cosf(y);  ry._m[0][1] = 0.0f; ry._m[0][2] = -sinf(y); ry._m[0][3] = 0.0f;
        ry._m[1][0] = 0.0f;     ry._m[1][1] = 1.0f; ry._m[1][2] = 0.0f;     ry._m[1][3] = 0.0f;
        ry._m[2][0] = sinf(y);  ry._m[2][1] = 0.0f; ry._m[2][2] = cosf(y);  ry._m[2][3] = 0.0f;
        ry._m[3][0] = 0.0f;     ry._m[3][1] = 0.0f; ry._m[3][2] = 0.0f;     ry._m[3][3] = 1.0f;

        //绕Z轴旋转z度
        rz._m[0][0] = cosf(z);  rz._m[0][1] = -sinf(z); rz._m[0][2] = 0.0f; rz._m[0][3] = 0.0f;
        rz._m[1][0] = sinf(z);  rz._m[1][1] = cosf(z);  rz._m[1][2] = 0.0f; rz._m[1][3] = 0.0f;
        rz._m[2][0] = 0.0f;     rz._m[2][1] = 0.0f;     rz._m[2][2] = 1.0f; rz._m[2][3] = 0.0f;
        rz._m[3][0] = 0.0f;     rz._m[3][1] = 0.0f;     rz._m[3][2] = 0.0f; rz._m[3][3] = 1.0f;

        *this = rz * ry * rx;
    }

    void LhMatrixFloat4::translation_transform(float x, float y, float z) {
        _m[0][0] = 1.0f; _m[0][1] = 0.0f; _m[0][2] = 0.0f; _m[0][3] = x;
        _m[1][0] = 0.0f; _m[1][1] = 1.0f; _m[1][2] = 0.0f; _m[1][3] = y;
        _m[2][0] = 0.0f; _m[2][1] = 0.0f; _m[2][2] = 1.0f; _m[2][3] = z;
        _m[3][0] = 0.0f; _m[3][1] = 0.0f; _m[3][2] = 0.0f; _m[3][3] = 1.0f;
    }

    void LhMatrixFloat4::lookat_left(LhVertexFloat3& eye, LhVertexFloat3& center, LhVertexFloat3& up) {
        LhVertexFloat3 f(eye - center);
        normalize(f);
        LhVertexFloat3 s(cross(up, f));
        normalize(s);
        LhVertexFloat3 u(cross(f, s));
        identity();
        /*_m[0][0] = s.get_x();
        _m[1][0] = s.get_y();
        _m[2][0] = s.get_z();
        _m[0][1] = u.get_x();
        _m[1][1] = u.get_y();
        _m[2][1] = u.get_z();
        _m[0][2] = f.get_x();
        _m[1][2] = f.get_y();
        _m[2][2] = f.get_z();
        _m[3][0] = -dot(s, eye);
        _m[3][1] = -dot(u, eye);
        _m[3][2] = -dot(f, eye);*/
        _m[0][0] = s.get_x(); _m[0][1] = s.get_y(); _m[0][2] = s.get_z(); _m[0][3] = -dot(s, eye);
        _m[1][0] = u.get_x(); _m[1][1] = u.get_y(); _m[1][2] = u.get_z(); _m[1][3] = -dot(u, eye);
        _m[2][0] = f.get_x(); _m[2][1] = f.get_y(); _m[2][2] = f.get_z(); _m[2][3] = -dot(f, eye);
        
    }

    void LhMatrixFloat4::coordinate_space_rotate(const LhVertexFloat3& target,
        const LhVertexFloat3& up) {
        LhVertexFloat3 N = target;
        LhVertexFloat3 U = up;

        normalize(N);
        U = cross(U, N);
        normalize(U);
        LhVertexFloat3 V = cross(N, U);

        _m[0][0] = U.get_x();   _m[0][1] = U.get_y();   _m[0][2] = U.get_z();   _m[0][3] = 0.0f;
        _m[1][0] = V.get_x();   _m[1][1] = V.get_y();   _m[1][2] = V.get_z();   _m[1][3] = 0.0f;
        _m[2][0] = N.get_x();   _m[2][1] = N.get_y();   _m[2][2] = N.get_z();   _m[2][3] = 0.0f;
        _m[3][0] = 0.0f;        _m[3][1] = 0.0f;        _m[3][2] = 0.0f;        _m[3][3] = 1.0f;
    }

    void LhMatrixFloat4::persproj_transform(const PersProjInfo& p) {
        const float ar = p.width / p.height;
        const float zrange = p.z_near - p.z_far;
        const float tan_half_fov = tanf(ToRadianF(p.fov / 2.0f));

        _m[0][0] = 1.0f / (tan_half_fov * ar); _m[0][1] = 0.0f;            _m[0][2] = 0.0f;            _m[0][3] = 0.0;
        _m[1][0] = 0.0f;                   _m[1][1] = 1.0f / tan_half_fov; _m[1][2] = 0.0f;            _m[1][3] = 0.0;
        _m[2][0] = 0.0f;                   _m[2][1] = 0.0f;            _m[2][2] = (-p.z_near - p.z_far) / zrange; _m[2][3] = 2.0f*p.z_far*p.z_near / zrange;
        _m[3][0] = 0.0f;                   _m[3][1] = 0.0f;            _m[3][2] = 1.0f;            _m[3][3] = 0.0;
    }

    void LhMatrixFloat4::orthoproj_transform(const OrthoProjInfo& p) {
        const float l = p.l;
        const float r = p.r;
        const float b = p.b;
        const float t = p.t;
        const float n = p.n;
        const float f = p.f;

        _m[0][0] = 2.0f / (r - l);  _m[0][1] = 0.0f;            _m[0][2] = 0.0f;            _m[0][3] = -(r + l) / (r - l);
        _m[1][0] = 0.0f;            _m[1][1] = 2.0f / (t - b);  _m[1][2] = 0.0f;            _m[1][3] = -(t + b) / (t - b);
        _m[2][0] = 0.0f;            _m[2][1] = 0.0f;            _m[2][2] = 2.0f / (f - n);  _m[2][3] = -(f + n) / (f - n);
        _m[3][0] = 0.0f;            _m[3][1] = 0.0f;            _m[3][2] = 0.0f;            _m[3][3] = 1.0;
    }
}
