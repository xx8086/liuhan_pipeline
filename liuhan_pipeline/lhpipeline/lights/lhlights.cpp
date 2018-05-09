#include "lhlights.h"
#include <math.h>

namespace lh_pipeline {

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

    LhVertexFloat3 LhLights::dir_light_factor(LhVertexFloat3& lights_negative_dir/*光射入反方向*/, LhVertexFloat3& normal, LhVertexFloat3& fragpos) {
        float accept = dot(lights_negative_dir, normal);//漫反射
        float spec = 0.0f;
        if (accept > 0) {
            LhVertexFloat3 reflecdir = reflect(-lights_negative_dir, normal);
            LhVertexFloat3 viewdir = fragpos - _viewpos;
            normalize(viewdir);
            spec = dot(reflecdir, -viewdir);
            if (spec > 0) {
                spec = pow(spec, _material.shininess);//镜面反射
            }
            else {
                spec = 0;
            }
        }

        LhVertexFloat3 dircolor = _material.ambient * _ambient_strength +
            _material.diffuse * (_diffuse_strength * accept) +
            _material.specular * (_specular_strength * spec);

        return _lightcolor * dircolor;
    }

    LhVertexFloat3 LhLights::point_light_factor(LhVertexFloat3& normal, LhVertexFloat3& fragpos) {
        LhVertexFloat3 light_dir = _light_pos - fragpos;
        normalize(light_dir);
        LhVertexFloat3 light_factor = dir_light_factor(light_dir, normal, fragpos);
        
        float diatance = length_float3(light_dir);
        float attenuation = 1.0f / (_kc + _kl * diatance + _kq * diatance * diatance);
        return light_factor * attenuation;
    }

    LhVertexFloat3 LhLights::spot_light_factor(LhVertexFloat3& normal, LhVertexFloat3& fragpos) {
        LhVertexFloat3 light_factor = point_light_factor(normal, fragpos);
        
        LhVertexFloat3 lights_dir = _light_pos - fragpos;
        normalize(lights_dir);
        float theta = dot(lights_dir, -lights_dir);
        float eosilon = _spot_inner - _spot_outer;
        float intensity = lh_clamp((theta - _spot_outer) / eosilon, 0.0f, 1.0f);
        return light_factor * intensity;
    }

	LhVertexFloat3 LhLights::dirlight(LhVertexFloat3 tex_color, LhVertexFloat3 normal, LhVertexFloat3 fragpos) {
		LhVertexFloat3 color = tex_color / 255.0f;
		color = color * dir_light_factor(_lights_negative_dir, normal, fragpos);
		return color * 255.0f;
	}

	LhVertexFloat3 LhLights::pointlight(LhVertexFloat3 tex_color, LhVertexFloat3 normal, LhVertexFloat3 fragpos) {
		LhVertexFloat3 color = tex_color / 255.0f;
        color = color * point_light_factor(normal, fragpos);
		return color * 255.0f;
    }

	LhVertexFloat3 LhLights::spotlight(LhVertexFloat3 tex_color, LhVertexFloat3 normal, LhVertexFloat3 fragpos) {
		LhVertexFloat3 color = tex_color / 255.0f;
        color = color * spot_light_factor(normal, fragpos);
		return color * 255.0f;
    }

    void LhLights::set_light_color(LhVertexFloat3 color, LhVertexFloat3 light_dir, LhVertexFloat3 viewpos, float ambient, float diff, float spec) {
        _ambient_strength = ambient;
        _diffuse_strength = diff;
        _specular_strength = spec;
        _lights_negative_dir = -light_dir;
		normalize(_lights_negative_dir);
		_viewpos = viewpos;
        _lightcolor.set_x(color.get_x() / 255.0f);
        _lightcolor.set_y(color.get_y() / 255.0f);
        _lightcolor.set_z(color.get_z() / 255.0f);
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

	void LhLights::set_material(LhMaterial& m) {
		_material = m;
	}
}
