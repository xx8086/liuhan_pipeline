#pragma once
#include "..\lhmath\lhdetail\lhvertex.h"

namespace lh_pipeline {
    typedef enum ViewWard {
        VIEW_FORWARD,
        VIEW_BACKWARD,
        VIEW_LEFT,
        VIEW_RIGHT,
    }VIEWWARD;

    class LhCamera {
    public:
        LhCamera();
        ~LhCamera();
    public:
        void update_camera_vectors();
        void set_front(float xoffset, float yoffset, bool constrainpitch = true);
        void set_view_ward(VIEWWARD direction, float deltatime);
    public:
        LhVertexFloat3 _pos;
        LhVertexFloat3 _target;
        LhVertexFloat3 _up;

        LhVertexFloat3 _world_up;
        LhVertexFloat3 _right;
        LhVertexFloat3 _front;
        float _yaw = -90.0f;
        float _pitch = 0.0f;
        float _view_speed = 5.0f;
        float _sensitivity = 0.1f;
    };

}

