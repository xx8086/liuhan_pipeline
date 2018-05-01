#pragma once
#include "..\lhmath\lhmath_common.h"
#include "..\lhmath\lhdetail\lhvertex.h"
namespace lh_pipeline {
    typedef enum LightType {
        LIGHT_TYPE_AMBIENT = 0,
        LIGHT_TYPE_POINT = 1,
        LIGHT_TYPE_SIMPLE_SPOT = 2,
        LIGHT_TYPE_SPOT = 3,
    }LIGHT_TYPE;

    class LhLights
    {
    public:
        LhLights(LIGHT_TYPE lighttype, unsigned int id);
        LhLights() {}
        ~LhLights();
    public:
        LhVertexFloat3 get_normal(LhVertexFloat3 a, LhVertexFloat3 b, LhVertexFloat3 c);
        lh_color get_dirlight(LhVertexFloat3 normal, LhVertexFloat3 view);
        lh_color get_pointlight(LhVertexFloat3 normal, LhVertexFloat3 view, LhVertexFloat3 fragpos);
        lh_color get_spotlight(LhVertexFloat3 normal, LhVertexFloat3 view, LhVertexFloat3 fragpos);
        void set_type(LIGHT_TYPE lighttype, unsigned int id);
        void set_light_color(lh_color color, LhVertexFloat3 light_di, float ambient, float diff, float spec, float shininess = 32);
        void set_point(LhVertexFloat3 light_pos, float kc, float kl, float kq);
        void set_spot(float inner, float outter, float umbra, float penumbra, float falloff);
        void visible() { _visible = true; };
        void invisible() { _visible = false; };
        bool is_visible() { return _visible; };
    private:
        LhVertexFloat3 reflect(LhVertexFloat3 I, LhVertexFloat3 N);
    private:
        unsigned int _id;
        bool _visible = false;
        float _shininess = 32;//聚光灯指数因子
        float _ambient_strength;
        float _diffuse_strength;
        float _specular_strength;

        float _kc;//衰减因子
        float _kl;
        float _kq;
        float _spot_inner;
        float _spot_outer;
        float _umbra;
        float _penumbra;
        float _falloff;

        LIGHT_TYPE _type;
        LhVertexFloat3 _lightcolor;
        LhVertexFloat3 _light_pos;
        LhVertexFloat3 _lights_dir;
        //LhVertexFloat3 _view_pos;
    };

}

