#include "lhmaterial.h"

namespace lh_pipeline {
	LhMaterial::LhMaterial()
	{
	}

	LhMaterial::~LhMaterial()
	{
	}

	LhMaterial::LhMaterial(LhVertexFloat3 a, LhVertexFloat3 d, LhVertexFloat3 f, float s) :
		ambient(a), diffuse(d), specular(f), shininess(s){
	}
}