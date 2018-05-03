#include "lhcamera.h"
#include "../lhmath/lhmath_common.h"
#include "../lhmath/lhdetail/lhquaternion.h"

namespace lh_pipeline {
    const static float STEP_SCALE = 1.0f;
    const static float EDGE_STEP = 0.5f;
    const static int MARGIN = 10;

    LhCamera::~LhCamera()
    {
    }

    LhCamera::LhCamera() :
        _pos(0.0f, 0.0f, 0.0f),
        _target(LhVertexFloat3(0.0f, 0.0f, 1.0f)),
        _up(LhVertexFloat3(0.0f, 1.0f, 0.0f)),
        _view_speed(0.1f) {
        init();
    }

    void LhCamera::set_pos(int w, int h, LhVertexFloat3 pos, LhVertexFloat3 target, LhVertexFloat3 up) {
        _width = w;
        _height = h;
        _pos = pos;
        _target = target;
        _up = up;
        normalize(_target);
        normalize(up);
        init();
    }

    void LhCamera::set_front_begin(float x, float y) {
        _last_x = x;
        _last_y = y;
    }

    void LhCamera::set_front(float x, float y){
        const int delta_x = x - _last_x;
        const int delta_y = y - _last_y;
        _last_x = x;
        _last_y = y;

        _angle_h -= (float)delta_x / 16.0f;
        _angle_v -= (float)delta_y / 16.0f;
        if (delta_x == 0) {
            if (x <= MARGIN) {
                _angle_h -= 1.0f;
                _left_edge = true;
            }
            else if (x >= (_width - MARGIN)) {
                _angle_v += 1.0f;
                _right_edge = true;
            }
        }
        else {
            _left_edge = false;
            _right_edge = false;
        }

        if (delta_y == 0) {
            if (y <= MARGIN) {
                _upper_edge = true;
            }
            else if (y >= (_height - MARGIN)) {
                _lower_edge = true;
            }
        }
        else {
            _upper_edge = false;
            _lower_edge = false;
        }

        update_camera_vectors();
    }


    void LhCamera::set_view_ward(VIEWWARD direction, float deltatime) {
        float velocity = _view_speed * deltatime;
        switch (direction) {
        case VIEW_FORWARD:
            _pos.set_z(_pos.get_z() + velocity);
            break;
        case VIEW_BACKWARD:
            _pos.set_z(_pos.get_z() - velocity);
            break;
        case VIEW_LEFT:
            _pos.set_x(_pos.get_x() - velocity);
            break;
        case VIEW_RIGHT:
            _pos.set_x(_pos.get_x() + velocity);
            break;
        default:break;
        }
    }

    void LhCamera::update_camera_vectors() {
        const LhVertexFloat3 vaxis(0.0f, 1.0f, 0.0f);
        LhVertexFloat3 view(1.0f, 0.0f, 0.0f);
        LhQuaternion quaternion;        
        view = quaternion.rotate(_angle_h, vaxis, view);
        normalize(view);

        LhVertexFloat3 haxis = cross(vaxis, view);
        normalize(haxis);
        _target = quaternion.rotate(_angle_v, haxis, view);
        normalize(_target);

        _up = cross(_target, haxis);
        normalize(_up);
    }

    void LhCamera::init() {
        LhVertexFloat3 HTarget(_target.get_x(), 0.0, _target.get_z());
        normalize(HTarget);

        if (HTarget.get_z() >= 0.0f){
            _angle_h = HTarget.get_x() >= 0.0f ? 
                360.0f - TDegreeF(asin(HTarget.get_z())) : 
                180.0f + TDegreeF(asin(HTarget.get_z()));
        }
        else{
            _angle_h = HTarget.get_x() >= 0.0f ? 
                TDegreeF(asin(-HTarget.get_z())) :
                TDegreeF(asin(-HTarget.get_z()));
        }

        _angle_v = -TDegreeF(asin(_target.get_y()));
        _upper_edge = false;
        _lower_edge = false;
        _left_edge = false;
        _right_edge = false;
        _last_x = _width / 2.0f;
        _last_y = _height / 2.0f;
    }


    void LhCamera::render()
    {
        bool shouldupdate = false;
        if (_left_edge) {
            _angle_h -= EDGE_STEP;
            shouldupdate = true;
        }
        else if (_right_edge) {
            _angle_h += EDGE_STEP;
            shouldupdate = true;
        }

        if (_upper_edge) {
            if (_angle_v > -90.0f) {
                _angle_v -= EDGE_STEP;
                shouldupdate = true;
            }
        }
        else if (_lower_edge) {
            if (_angle_v < 90.0f) {
                _angle_v += EDGE_STEP;
                shouldupdate = true;
            }
        }

        if (shouldupdate) {
            update_camera_vectors();
        }
    }
            
}
