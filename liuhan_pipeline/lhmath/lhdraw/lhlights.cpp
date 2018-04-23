#include "lhlights.h"
#include <math.h>

namespace lh_pipeline {
#define MAX(a, b) (a > b ? a : b) 

    LhLights::LhLights(LIGHT_TYPE lighttype, unsigned int id):
        _type(lighttype), _id(id){
    }


    LhLights::~LhLights()
    {
    }

    void LhLights::set_type(LIGHT_TYPE lighttype, unsigned int id) {
        _type = lighttype;
        _id = id;
    }

    LhVertexFloat3 LhLights::reflect(LhVertexFloat3 I, LhVertexFloat3 N) {        float cos_theta = dot(I, N);        LhVertexFloat3 R = N * (cos_theta * 2) - I;        if (cos_theta > 0) {            normalize(R);        }        else {            R = LhVertexFloat3(0.0, 0.0, 0.0);        }        return R;    }

    lh_color LhLights::get_dirlight(LhVertexFloat3 normal, LhVertexFloat3 viewdir) {
        normalize(normal);
        normalize(viewdir);
        LhVertexFloat3 reflecdir = reflect(_lights_dir, normal);
        float diff = MAX(dot(normal, _lights_dir), 0.0f);
        float spec = pow(MAX(dot(viewdir, reflecdir), 0.0), _shininess);

        LhVertexFloat3 ambient = _lightcolor * _ambient_strength;// vec3(texture(material.diffuse, TexCoords));
        LhVertexFloat3 diffuse = _lightcolor * _diffuse_strength * diff;// *vec3(texture(material.diffuse, TexCoords));
        LhVertexFloat3 specular = _lightcolor * _specular_strength * spec;// *vec3(texture(material.specular, TexCoords));
        return lh_color(ambient.get_x() + diffuse.get_x() + specular.get_x(),
            ambient.get_y() + diffuse.get_y() + specular.get_y(),
            ambient.get_z() + diffuse.get_z() + specular.get_z());
    }

    lh_color LhLights::get_pointlight(LhVertexFloat3 normal, LhVertexFloat3 viewdir, LhVertexFloat3 fragpos) {
        normalize(normal);
        normalize(viewdir);
        LhVertexFloat3 reflecdir = reflect(_lights_dir, normal);
        float diff = MAX(dot(normal, _lights_dir), 0.0f);
        float spec = pow(MAX(dot(viewdir, reflecdir), 0.0), _shininess);

        float ambient_x = _lightcolor.get_x() * _ambient_strength;
        float ambient_y = _lightcolor.get_y() * _ambient_strength;
        float ambient_z = _lightcolor.get_z() * _ambient_strength;
        float diffuse_x = _lightcolor.get_x() * _diffuse_strength * diff;
        float diffuse_y = _lightcolor.get_y() * _diffuse_strength * diff;
        float diffuse_z = _lightcolor.get_z() * _diffuse_strength * diff;
        float specular_x = _lightcolor.get_x() * _specular_strength * spec;
        float specular_y = _lightcolor.get_y() * _specular_strength * spec;
        float specular_z = _lightcolor.get_z() * _specular_strength * spec;
        float color_x = ambient_x + diffuse_x + specular_x;
        float color_y = ambient_y + diffuse_y + specular_y;
        float color_z = ambient_z + diffuse_z + specular_z;

        LhVertexFloat3 distance = _light_pos - fragpos;
        float distance_x = distance.get_x();
        float distance_y = distance.get_y();
        float distance_z = distance.get_z();
        float point_distance = sqrt(distance_x * distance_x + distance_y * distance_y + distance_z * distance_z);
        float attenuation = 1.0f / (_kc + _kl * point_distance + _kq * point_distance * point_distance);
        return lh_color(color_x*attenuation, color_y * attenuation, color_z * attenuation);
    }


    lh_color LhLights::get_spotlight(LhVertexFloat3 normal, LhVertexFloat3 viewdir, LhVertexFloat3 fragpos) {
        lh_color color = get_pointlight(normal, viewdir, fragpos);
        /*
        LhVertexFloat3 light_to_frag_dir = fragpos - _light_pos;
        float theta = dot(light_to_frag_dir, _lights_dir);
        float epsilon = _spot_inner - _spot_outer;
        float intensity = std::clamp((theta - _spot_outer) / epsilon, 0.0, 1.0);
        return color * intensity;*/
        return color;
    }

    void LhLights::set_light_color(lh_color color, LhVertexFloat3 light_dir, float ambient, float diff, float spec, float shininess) {
        _shininess = shininess;//聚光灯指数因子
        _ambient_strength = ambient;
        _diffuse_strength = diff;
        _specular_strength = spec;
        _lights_dir = light_dir;
        _lightcolor = LhVertexFloat3(color.red, color.green, color.blue);
    }
    void LhLights::set_point(LhVertexFloat3 light_pos, float kc, float kl, float kq) {
        _light_pos = light_pos;
        _kc = kc;//衰减因子
        _kl = kl;
        _kq = kq;        
    }
    void LhLights::set_spot(float inner, float outter, float umbra, float penumbra, float falloff) {
        _spot_inner = inner;
        _spot_outer = outter;
        _umbra = umbra;
        _penumbra = penumbra;
        _falloff = falloff;
    }

    LhVertexFloat3 LhLights::get_normal(LhVertexFloat3 a, LhVertexFloat3 b, LhVertexFloat3 c) {
        LhVertexFloat3 n(0.0f, 0.0f, 0.0f);
        if (_visible) {
            LhVertexFloat3 ab = b - a;
            LhVertexFloat3 ac = c - a;
            n = cross(ab, ac);
        }
        
        return n;
    }
}
