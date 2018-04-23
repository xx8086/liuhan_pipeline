#include "lhlights.h"
#include <math.h>

namespace lh_pipeline {
    LhLights::LhLights(LIGHT_TYPE lighttype, int id):
        _type(lighttype), _id(id){
    }


    LhLights::~LhLights()
    {
    }

    float LhLights::max(float a, float b) {
        return a > b ? a : b;
    }

    LhVertexFloat3 LhLights::reflect(LhVertexFloat3 I, LhVertexFloat3 N) {        float cos_theta = dot(I, N);        LhVertexFloat3 R = N * (cos_theta * 2) - I;        if (cos_theta > 0) {            normalize(R);        }        else {            R = LhVertexFloat3(0.0, 0.0, 0.0);        }        return R;    }

    lh_color LhLights::get_dirlight(LhVertexFloat3 normal, LhVertexFloat3 viewdir) {
        normalize(normal);
        normalize(viewdir);
        LhVertexFloat3 reflecdir = reflect(_lights_dir, normal);
        float diff = max(dot(normal, _lights_dir), 0.0f);
        float spec = pow(max(dot(viewdir, reflecdir), 0.0), _shininess);

        LhVertexFloat3 ambient = _lightcolor * _ambient_strength;// vec3(texture(material.diffuse, TexCoords));
        LhVertexFloat3 diffuse = _lightcolor * _diffuse_strength * diff;// *vec3(texture(material.diffuse, TexCoords));
        LhVertexFloat3 specular = _lightcolor * _specular_strength * spec;// *vec3(texture(material.specular, TexCoords));
        LhVertexFloat3 dir = ambient + diffuse + specular;
        return lh_color(ambient.get_x() + diffuse.get_x() + specular.get_x(),
            ambient.get_y() + diffuse.get_y() + specular.get_y(),
            ambient.get_z() + diffuse.get_z() + specular.get_z());
    }

    lh_color LhLights::get_pointlight(LhVertexFloat3 normal, LhVertexFloat3 viewdir, LhVertexFloat3 fragpos) {
        lh_color color = get_dirlight(normal, viewdir);
        float point_distance = distance(_light_pos, fragpos);
        float attenuation = 1.0f / (_kc + _kl * point_distance + _kq * point_distance * point_distance);
        return color * attenuation;
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
}
