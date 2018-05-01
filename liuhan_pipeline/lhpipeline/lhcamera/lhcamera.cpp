#include "lhcamera.h"
#include "../lhmath/lhmath_common.h"

namespace lh_pipeline {
    LhCamera::~LhCamera()
    {
    }

    LhCamera::LhCamera() :
        _front(LhVertexFloat3(0.0f, 0.0f, -1.0f)),
        _world_up(LhVertexFloat3(0.0f, 1.0f, 0.0f)),
        _yaw(90.0f),
        _pitch(0.0f),
        _sensitivity(0.1f),
        _view_speed(15.0f) {
        update_camera_vectors();
    }

    void LhCamera::set_front(float xoffset, float yoffset, bool constrainpitch){
        xoffset *= _sensitivity;
        yoffset *= _sensitivity;

        _yaw += xoffset;
        _pitch += yoffset;

        if (constrainpitch){
            if (_pitch > 89.0f)_pitch = 89.0f;
            if (_pitch < -89.0f)_pitch = -89.0f;
        }
        update_camera_vectors();
    }

    void LhCamera::set_view_ward(VIEWWARD direction, float deltatime) {
        float velocity = _view_speed * deltatime;
        switch (direction) {
        case VIEW_FORWARD:
            _pos = (_pos - (_front * velocity));
            break;
        case VIEW_BACKWARD:
            _pos = (_pos + (_front * velocity));
            break;
        case VIEW_LEFT:
            _pos = (_pos - (_right * velocity));
            break;
        case VIEW_RIGHT:
            _pos = (_pos + (_right * velocity));
            break;
        default:break;
        }
    }

    void LhCamera::update_camera_vectors() {
        _front = LhVertexFloat3 (
            cos(ToRadianF(_yaw)) * cos(ToRadianF(_pitch)),
            sin(ToRadianF(_pitch)),
            sin(ToRadianF(_yaw)) * cos(ToRadianF(_pitch))
        );
        normalize(_front);
        // Also re-calculate the Right and Up vector
        _right = cross(_front, _world_up);
        normalize(_right);
        _up = cross(_right, _front);
        normalize(_up);
        _target = _front + _pos;
    }
}
