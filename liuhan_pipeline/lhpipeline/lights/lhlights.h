#pragma once
#include "..\lhmath\lhmath_common.h"
#include "..\lhmath\lhdetail\lhvertex.h"
#include "lhmaterial.h"

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
		LhVertexFloat3 dirlight(LhVertexFloat3 tex_color, LhVertexFloat3 normal, LhVertexFloat3 fragpos);
		LhVertexFloat3 pointlight(LhVertexFloat3 tex_color, LhVertexFloat3 normal,  LhVertexFloat3 fragpos);
		LhVertexFloat3 spotlight(LhVertexFloat3 tex_color, LhVertexFloat3 normal,  LhVertexFloat3 fragpos);
		void set_material(LhMaterial&);
		void set_type(LIGHT_TYPE lighttype, unsigned int id);
        void set_light_color(LhVertexFloat3 color, LhVertexFloat3 light_di, LhVertexFloat3 viewpos, float ambient, float diff, float spec);
        void set_point(LhVertexFloat3 light_pos, float kc, float kl, float kq);
        void set_spot(float inner, float outter, float umbra, float penumbra, float falloff);
        void visible() { _visible = true; };
        void invisible() { _visible = false; };
        bool is_visible() { return _visible; };
    private:
        LhVertexFloat3 dir_light_factor(LhVertexFloat3& lights_negative_dir, LhVertexFloat3& normal, LhVertexFloat3& fragpos);
        LhVertexFloat3 point_light_factor(LhVertexFloat3& normal, LhVertexFloat3& fragpos);
        LhVertexFloat3 spot_light_factor(LhVertexFloat3& normal, LhVertexFloat3& fragpos);
    private:
        unsigned int _id;
        bool _visible = false;
        float _ambient_strength;
        float _diffuse_strength;
        float _specular_strength;

        float _kc;//Ë¥¼õÒò×Ó
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
        LhVertexFloat3 _lights_negative_dir;
		LhVertexFloat3 _viewpos;
		LhMaterial _material;
    };

}

