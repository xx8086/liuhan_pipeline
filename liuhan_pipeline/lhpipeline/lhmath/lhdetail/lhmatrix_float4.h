#pragma once
#include "lhvertex.h"

namespace lh_pipeline {

    struct PersProjInfo{
        float fov;
        float width;
        float height;
        float z_near;
        float z_far;
        PersProjInfo() {}
        PersProjInfo(float f, float w, float h, float near, float far) :
            fov(f), width(w), height(h), z_near(near), z_far(far) {
            ;
        }
    };

    struct OrthoProjInfo
    {
        float r;        // right
        float l;        // left
        float b;        // bottom
        float t;        // top
        float n;        // z near
        float f;        // z far
    };

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
        void identity();
        float determinant() const;
        LhMatrixFloat4& inverse();
        LhMatrixFloat4 transpose() ;
        LhMatrixFloat4 operator*(const LhMatrixFloat4& right);
        LhVertexFloat4 operator*(const LhVertexFloat4& right);
        LhVertexFloat4 operator*(const LhVertexFloat3& right);//w = 1
        
    public:
        void scale_transform(float, float, float);
        void rotate_transform(float, float, float);
        void translation_transform(float, float, float);
        void persproj_transform(const PersProjInfo& p);
        void orthoproj_transform(const OrthoProjInfo& p);
        void coordinate_space_rotate(const LhVertexFloat3& target, const LhVertexFloat3& up);
        void lookat_left(LhVertexFloat3& eys, LhVertexFloat3& center, LhVertexFloat3& up);
    protected:
        float _m[4][4];
    };
}
