#pragma once

class LhMatrixFloat4
{
public:
    LhMatrixFloat4();
    ~LhMatrixFloat4();
public:
    LhMatrixFloat4(float a00, float a01, float a02, float a03,
        float a10, float a11, float a12, float a13,
        float a20, float a21, float a22, float a23,
        float a30, float a31, float a32, float a33);
public:
    void set_zero();
    void init_identity();
    float determinant() const;
    LhMatrixFloat4& inverse();
    LhMatrixFloat4 transpose() const;
    LhMatrixFloat4 operator*(const LhMatrixFloat4& Right) const;
public:
    float _m[4][4];
};

