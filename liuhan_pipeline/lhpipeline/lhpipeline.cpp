#include "lhpipeline.h"


namespace lh_pipeline {
    LhPipeLine::LhPipeLine()
    {
    }


    LhPipeLine::~LhPipeLine()
    {
    }

    void LhPipeLine::set_view_ward(VIEWWARD direction, float deltatime) {
        _camera.set_view_ward(direction, deltatime);
    }

    void LhPipeLine::set_front_begin(float xoffset, float yoffset) {
        
    }
    void LhPipeLine::set_front(float xoffset, float yoffset) {
        _camera.set_front(xoffset, yoffset);
    }
    void LhPipeLine::LhPipeLine::set_rotate(float x, float y, float z) {
        _rotateinfo.set_x(x);
        _rotateinfo.set_y(y);
        _rotateinfo.set_z(z);
    }
    void LhPipeLine::set_sale(float x, float y, float z) {
        _scale.set_x(x);
        _scale.set_y(y);
        _scale.set_z(z);
    }
    void LhPipeLine::set_worldpos(float x, float y, float z) {
        _worldpos.set_x(x);
        _worldpos.set_y(y);
        _worldpos.set_z(z);
    }

    LhVertexFloat3& LhPipeLine::get_view_pos() {
        return _camera._pos;
    }
    LhVertexFloat3& LhPipeLine::get_view_dir() {
        return _camera._target;
    }

    void LhPipeLine::set_camera_pos(LhVertexFloat3 pos) {
        _camera._pos = pos;
    }
    void LhPipeLine::set_orthographic_proj(const OrthoProjInfo& o) {
        _orthoprojinfo = o;
    }
    void LhPipeLine::set_perspective_proj(const PersProjInfo& p) {
        _pers_projinfo = p;
    }

    const LhMatrixFloat4& LhPipeLine::get_world() {
        return _world_transformation;
    }
    const LhMatrixFloat4& LhPipeLine::get_view() {
        return _view_transformation;
    }
    const LhMatrixFloat4& LhPipeLine::get_proj() {
        return _proj_transformation;
    }
    const LhMatrixFloat4& LhPipeLine::get_wvp() {
        init_world();
        init_view();
        init_proj();
        _wvp_transformation = _proj_transformation * _view_transformation * _world_transformation;
        return _wvp_transformation;
    }

    const LhMatrixFloat4& LhPipeLine::get_ortho() {
        return _ortho_transformation;
    }

    const LhMatrixFloat4& LhPipeLine::get_wvo() {
        init_world();
        init_view();
        init_ortho();
        _wvo_transformation = _ortho_transformation * _view_transformation * _world_transformation;
        return _wvo_transformation;
    }

    void LhPipeLine::init_world() {
        LhMatrixFloat4 scale_trans;
        LhMatrixFloat4 rotate_trans;
        LhMatrixFloat4 translation_trans;

        scale_trans.init_scale_transform(_scale.get_x(), _scale.get_y(), _scale.get_z());
        rotate_trans.init_rotate_transform(_rotateinfo.get_x(), _rotateinfo.get_y(), _rotateinfo.get_z());
        translation_trans.init_translation_transform(_worldpos.get_x(), _worldpos.get_y(), _worldpos.get_z());

        _world_transformation = translation_trans * rotate_trans * scale_trans;
    }
    void LhPipeLine::init_view() {
        LhMatrixFloat4 camera_translation_trans;
        LhMatrixFloat4 camera_rotate_trans;
        camera_translation_trans.init_translation_transform(
            -_camera._pos.get_x(),
            -_camera._pos.get_y(),
            -_camera._pos.get_z());//移到相机位置
        camera_rotate_trans.coordinate_space_rotate(_camera._target, _camera._up);//旋转和相机轴重合
        _view_transformation = camera_rotate_trans * camera_translation_trans;
    }
    void LhPipeLine::init_proj() {
        _proj_transformation.init_persproj_transform(_pers_projinfo);
    }
    void LhPipeLine::init_ortho() {
        _ortho_transformation.init_orthoproj_transform(_orthoprojinfo);
    }

    LhVertexFloat4 LhPipeLine::transformation_in_mvp(const LhVertexFloat3& v) {
        return _wvp_transformation *v;
    }

    LhVertexFloat4 LhPipeLine::transformation_in_mvo(const LhVertexFloat3& v) {
        return _wvo_transformation * v;
    }

    unsigned int LhPipeLine::transformation_cut(const LhVertexFloat4& v4) {
        unsigned int cut = 0;
        float w = v4.get_w();
        if (v4.get_z() < 0.0f)  cut |= 0x01;
        if (v4.get_z() > w)     cut |= 0x02;
        if (v4.get_x() < -w)    cut |= 0x04;
        if (v4.get_x() > w)     cut |= 0x08;
        if (v4.get_y() < -w)    cut |= 0x10;
        if (v4.get_y() > w)     cut |= 0x20;//0x40 0x80 0x100 0x200 0x400 0x800...
        return cut;
    }

    LhVertexFloat4 LhPipeLine::transformation_normalization(const LhVertexFloat4& v4) {
        LhVertexFloat4 r;
        float rhw = 1.0f / v4.get_w();
        r.set_x((1.0f + v4.get_x() * rhw) * _width * 0.5f);
        if (r.get_x() > 800) {
            int i =0;
        }
        r.set_y((1.0f - v4.get_y() * rhw) * _height * 0.5f);
        r.set_z(v4.get_z() * rhw);
        r.set_w(1.0f);
        return r;
    }

    void LhPipeLine::set_windows_size(int w, int h) {
        _width = w;
        _height = h;
    }
}
