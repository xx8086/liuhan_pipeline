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

	LhVertexFloat3 LhLights::dirlight(LhVertexFloat3 tex_color, LhVertexFloat3 normal, LhVertexFloat3 fragpos) {
        LhVertexFloat3 lights_dir = _light_pos - fragpos;
		normalize(lights_dir);
		float accept = dot(lights_dir, normal);//漫反射
		float spec = 0.0f;
		if (accept > 0) {
			LhVertexFloat3 reflecdir = reflect(-lights_dir, normal);
			LhVertexFloat3 viewdir = _viewpos - fragpos;
			normalize(viewdir);
			spec = dot(reflecdir, viewdir);
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

		dircolor = _lightcolor * dircolor;
		LhVertexFloat3 color = tex_color / 255.0f;
		color = color * dircolor;
		return color * 255.0f;
	}

	LhVertexFloat3 LhLights::pointlight(LhVertexFloat3 tex_color, LhVertexFloat3 normal, LhVertexFloat3 fragpos) {
		LhVertexFloat3 lights_dir = _light_pos - fragpos;
		normalize(lights_dir);
		float accept = dot(lights_dir, normal);//漫反射
		float spec = 0.0f;
		if (accept > 0) {
			LhVertexFloat3 reflecdir = reflect(-lights_dir, normal);
			LhVertexFloat3 viewdir = _viewpos - fragpos;
			normalize(viewdir);
			spec = dot(reflecdir, viewdir);
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

        LhVertexFloat3 distance = _light_pos - fragpos;
        float distance_x = distance.get_x();
        float distance_y = distance.get_y();
        float distance_z = distance.get_z();
        float point_distance = sqrt(distance_x * distance_x + distance_y * distance_y + distance_z * distance_z);
        float attenuation = 1.0f / (_kc + _kl * point_distance + _kq * point_distance * point_distance);
		LhVertexFloat3  point  = dircolor * attenuation;

		point = _lightcolor * point;
		LhVertexFloat3 color = tex_color / 255.0f;
		color = color * point;
		return color * 255.0f;
    }

	LhVertexFloat3 LhLights::spotlight(LhVertexFloat3 tex_color, LhVertexFloat3 normal, LhVertexFloat3 fragpos) {
        LhVertexFloat3 lights_dir = _light_pos - fragpos;
		normalize(lights_dir);
		float accept = dot(lights_dir, normal);//漫反射
		float spec = 0.0f;
		if (accept > 0) {
			LhVertexFloat3 reflecdir = reflect(-lights_dir, normal);
			LhVertexFloat3 viewdir = _viewpos - fragpos;
			normalize(viewdir);
			spec = dot(reflecdir, viewdir);
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

		LhVertexFloat3 distance = _light_pos - fragpos;
		float distance_x = distance.get_x();
		float distance_y = distance.get_y();
		float distance_z = distance.get_z();
		float point_distance = sqrt(distance_x * distance_x + distance_y * distance_y + distance_z * distance_z);
		float attenuation = 1.0f / (_kc + _kl * point_distance + _kq * point_distance * point_distance);

		LhVertexFloat3 lights_dir2 = _lights_dir;
		normalize(lights_dir2);
		float theta = dot(lights_dir, -lights_dir2);
		float eosilon = _spot_inner - _spot_outer;
		float intensity = lh_clamp((theta - _spot_outer) / eosilon, 0.0f, 1.0f);
		LhVertexFloat3  spot = dircolor * attenuation * intensity;

		spot = _lightcolor * spot;
		LhVertexFloat3 color = tex_color / 255.0f;
		color = color * spot;
		return color * 255.0f;
    }

    void LhLights::set_light_color(LhVertexFloat3 color, LhVertexFloat3 light_dir, LhVertexFloat3 viewpos, float ambient, float diff, float spec) {
        _ambient_strength = ambient;
        _diffuse_strength = diff;
        _specular_strength = spec;
		_lights_dir = light_dir;
		normalize(_lights_dir);
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
