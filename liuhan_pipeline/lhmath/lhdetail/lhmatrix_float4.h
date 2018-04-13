#pragma once
#include "lhvertex.h"

namespace lh_pipeline {

    struct PersProjInfo{
        float fov;
        float width;
        float height;
        float z_near;
        float z_far;
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
        void init_identity();
        float determinant() const;
        LhMatrixFloat4& inverse();
        LhMatrixFloat4 transpose() const;
        LhMatrixFloat4 operator*(const LhMatrixFloat4& Right) const;
    public:
        void init_scale_transform(float, float, float);
        void init_rotate_transform(float, float, float);
        void init_translation_transform(float, float, float);
        void init_camera_transform(const LhVertex<float, 3>& target, const LhVertex<float, 3>& up);
        void init_persproj_transform(const PersProjInfo& p);
        void init_orthoproj_transform(const OrthoProjInfo& p);
    protected:
        float _m[4][4];
    };
}
