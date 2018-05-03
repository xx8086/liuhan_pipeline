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
    void LhPipeLine::set_view_orientation(VIEWWARD direction, float deltatime) {
        _camera.set_view_orientation(direction, deltatime);
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

	bool LhPipeLine::can_draw() {
		return _camera._pos.get_z() < _worldpos.get_z();
	}

    LhVertexFloat3 LhPipeLine::get_look() {
        LhVertexFloat3 look = _camera._target + _camera._pos;
        normalize(look);
        return look;
    }
    LhVertexFloat3& LhPipeLine::get_view_pos() {
        return _camera._pos;
    }
    LhVertexFloat3& LhPipeLine::get_view_dir() {
        return _camera._target;
    }

    void LhPipeLine::set_camera_pos(LhVertexFloat3 pos, LhVertexFloat3 target, LhVertexFloat3 up) {
        _camera.set_pos(_width, _height, pos, target, up);
		_camera.update_camera_vectors();
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
    const LhMatrixFloat4& LhPipeLine::get_wv() {
        init_world();
        init_view();
        _wv_transformation = _view_transformation * _world_transformation;
        return _wv_transformation;
    }
    const LhMatrixFloat4& LhPipeLine::get_wvp() {
        get_wv();
        init_proj();
        _wvp_transformation = _proj_transformation * _wv_transformation;
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

        scale_trans.scale_transform(_scale.get_x(), _scale.get_y(), _scale.get_z());
        rotate_trans.rotate_transform(_rotateinfo.get_x(), _rotateinfo.get_y(), _rotateinfo.get_z());
        translation_trans.translation_transform(_worldpos.get_x(), _worldpos.get_y(), _worldpos.get_z());

        _world_transformation = translation_trans * rotate_trans * scale_trans;
    }
    void LhPipeLine::init_view() {
        LhMatrixFloat4 camera_translation_trans;
        LhMatrixFloat4 camera_rotate_trans;
        camera_translation_trans.translation_transform(
            -_camera._pos.get_x(),
            -_camera._pos.get_y(),
            -_camera._pos.get_z());//移到相机位置
        camera_rotate_trans.coordinate_space_rotate(_camera._target, _camera._up);//旋转和相机轴重合
        _view_transformation = camera_rotate_trans * camera_translation_trans;

        //_view_transformation.lookat_left(_camera._pos, _camera._target, _camera._up);
    }
    void LhPipeLine::init_proj() {
        _proj_transformation.persproj_transform(_pers_projinfo);
    }
    void LhPipeLine::init_ortho() {
        _ortho_transformation.orthoproj_transform(_orthoprojinfo);
    }

    LhVertexFloat4 LhPipeLine::transformation_in_mvp(const LhVertexFloat3& v) {
        return _wvp_transformation * v;
    }

    LhVertexFloat4 LhPipeLine::transformation_in_mvo(const LhVertexFloat3& v) {
        return _wvo_transformation * v;
    }

	bool LhPipeLine::transformation_cvv(const LhVertexFloat4& v4) {
        float w = v4.get_w();
		if (v4.get_z()  < -w || w < v4.get_z() ||
			v4.get_x() < -w  || w < v4.get_x()||
			v4.get_y() < -w || w < v4.get_y() ) {
			return false;
		}
        return true;
    }

    LhVertexFloat4 LhPipeLine::transformation_homogeneous(const LhVertexFloat4& x) {
		float rhw = 1.0f / x.get_w();
        LhVertexFloat4 r(
            x.get_x() * rhw,
            x.get_z() * rhw,
            x.get_z() * rhw,
            1.0f);
		return r;
    }

    void LhPipeLine::set_windows_size(int w, int h) {
        _width = w;
        _height = h;
    }
}
