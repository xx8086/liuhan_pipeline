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
        void render();
        void update_camera_vectors();
        void set_pos(int w, int h, LhVertexFloat3 pos, LhVertexFloat3 target, LhVertexFloat3 up);
        void set_front(float x, float y);
        void set_view_ward(VIEWWARD direction, float deltatime);
        void set_view_orientation(VIEWWARD direction, float deltatime);
    private:
        void init();
    public:
        LhVertexFloat3 _pos;
        LhVertexFloat3 _target;
        LhVertexFloat3 _up;
        float _view_speed = 5.0f;
        int _width = 0;
        int _height = 0;
        float _angle_h;
        float _angle_v;
        float _last_x;
        float _last_y;
        bool _upper_edge;
        bool _lower_edge;
        bool _left_edge;
        bool _right_edge;
    };

}

