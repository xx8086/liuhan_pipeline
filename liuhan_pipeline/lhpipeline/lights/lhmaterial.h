#pragma once
#include "..\lhmath\lhdetail\lhvertex.h"
namespace lh_pipeline {
	class LhMaterial
	{
	public:
		LhMaterial();
		LhMaterial(LhVertexFloat3 a, LhVertexFloat3 d, LhVertexFloat3 f, float s);
		~LhMaterial();
	public:
		LhVertexFloat3 ambient;
		LhVertexFloat3 diffuse;
		LhVertexFloat3 specular;
		float shininess;
	};
}
