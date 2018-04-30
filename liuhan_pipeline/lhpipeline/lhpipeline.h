#pragma once

#include "../lhmath/lhdetail/lhmatrix_float4.h"
#include "lhcamera\lhcamera.h"

namespace lh_pipeline {
    class LhPipeLine
    {
    public:
        LhPipeLine();
        ~LhPipeLine();
    public:
        void set_windows_size(int w, int h);
        LhVertexFloat4 transformation_in_mvp(const LhVertexFloat3&);
        LhVertexFloat4 transformation_in_mvo(const LhVertexFloat3&);
        LhVertexFloat4 transformation_homogeneous(const LhVertexFloat4&);
        unsigned int transformation_cut(const LhVertexFloat4&);
        void set_front_begin(float xoffset, float yoffset);
        void set_front(float xoffset, float yoffset);
    public:
        LhVertexFloat3& get_view_pos();
        LhVertexFloat3& get_view_dir();
        void set_rotate(float x, float y, float z);
        void set_sale(float x, float y, float z);
        void set_worldpos(float x, float y, float z);
        void set_camera_pos(LhVertexFloat3 pos);
        void set_view_ward(VIEWWARD direction, float deltatime);
        void set_orthographic_proj(const OrthoProjInfo& o);
        void set_perspective_proj(const PersProjInfo& p);
		bool can_draw();
    public:
        const LhMatrixFloat4& get_wvp();
        const LhMatrixFloat4& get_wvo();
        const LhMatrixFloat4& get_world();
        const LhMatrixFloat4& get_view();
        const LhMatrixFloat4& get_proj();
        const LhMatrixFloat4& get_ortho();
    private:
        void init_world();
        void init_view();
        void init_proj();
        void init_ortho();
    private:
        PersProjInfo _pers_projinfo;
        OrthoProjInfo _ortho_projinfo;

        LhVertexFloat3 _scale;
        LhVertexFloat3 _worldpos;
        LhVertexFloat3 _rotateinfo;

        PersProjInfo _persProjinfo;
        OrthoProjInfo _orthoprojinfo;

        LhMatrixFloat4 _wvp_transformation;
        LhMatrixFloat4 _wvo_transformation;
        LhMatrixFloat4 _world_transformation;
        LhMatrixFloat4 _view_transformation;
        LhMatrixFloat4 _proj_transformation;
        LhMatrixFloat4 _ortho_transformation;
        LhCamera _camera;
        int _width = 0;
        int _height = 0;
    };
}

