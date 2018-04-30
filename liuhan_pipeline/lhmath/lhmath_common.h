#pragma once
#include "lhdetail\lhvertex.h"

namespace lh_pipeline {

#define LH_PI (3.1415926f)

#define ToRadianF(degree) (((float)degree * LH_PI) / 180.0f) 
#define TDegreeF(radian) (((float)radian * 180.0f) / LH_PI)


	struct lh_color {
		float red = 0;
		float green = 0;
		float blue = 0;
		float alph = 0;
		lh_color() {}
		lh_color(float r, float g, float b, float a = 255) :
			red(r), green(g), blue(b) {}
		void check() {
			if (red > 255.0)red = 255.0;
			if (green > 255.0)green = 255.0;
			if (blue > 255.0)blue = 255.0;
			if (alph > 255.0)alph = 255.0;

			if (red < 0.0)red = 0.0;
			if (green < 0.0)green = 0.0;
			if (blue < 0.0)blue = 0.0;
			if (alph < 0.0)alph = 0.0;
		}
		template <typename T>
		lh_color(T rgba) {
			blue = (rgba) & 0xff;
			green = (rgba >> 8) & 0xff;
			red = (rgba >> 16) & 0xff;
			alph = (rgba >> 24) & 0xff;
		}

		template <typename T>
		T get_t() {
			return T((int)blue | (int)green << 8 | (int)red << 16 | (int)alph << 24);
		}

		lh_color& operator+=(const lh_color &v) {
			alph += v.alph;
			red += v.red;
			green += v.green;
			blue += v.blue;
			return *this;
		}

		lh_color operator+ (const lh_color &v) {
			return lh_color(red + v.red,
				green + v.green,
				blue + v.blue,
				alph + v.alph);
		}

		lh_color operator+ (const lh_color &v) const {
			return lh_color(red + v.red,
				green + v.green,
				blue + v.blue,
				alph + v.alph);
		}

		lh_color operator- (const lh_color &v) const {
			return lh_color(red - v.red,
				green - v.green,
				blue - v.blue,
				alph - v.alph);
		}

		lh_color operator- (const lh_color &v) {
			return lh_color(red - v.red,
				green - v.green,
				blue - v.blue,
				alph - v.alph);
		}

		lh_color operator+ (float v) {
			return lh_color(red + v,
				green + v,
				blue + v,
				alph + v);
		}

		lh_color operator* (float v) {
			return lh_color(red * v,
				green * v,
				blue * v,
				alph * v);
		}

		lh_color operator/ (float v) {
			return lh_color(red / v,
				green / v,
				blue / v,
				alph / v);
		}
	};



	struct TextureUV
	{
		TextureUV() {}
		TextureUV(float _u, float _v) :u(_u), v(_v) {}
		float u = 0;
		float v = 0;

		TextureUV operator*(const float& other) {
			return TextureUV(u * other, v * other);
		}
		TextureUV operator-(const TextureUV &otherv) {
			return TextureUV(u - otherv.u, v - otherv.v);
		}

		TextureUV operator-(const TextureUV &otherv) const {
			return TextureUV(u - otherv.u, v - otherv.v);
		}

		TextureUV operator+(const float otherv) {
			return TextureUV(u + otherv, v + otherv);
		}

		TextureUV operator+(const TextureUV &otherv) {
			return TextureUV(u + otherv.u, v + otherv.v);
		}

		TextureUV operator+(const TextureUV &otherv) const {
			return TextureUV(u + otherv.u, v + otherv.v);
		}
		/*                1
		0.----------------.-----> +u
		|                         |
		|                         |
		|                         |
		|                         |
		1 |.----------------.(texure_size, texure_size)
		\/
		+v   */
	};


	struct VertexColor {
		LhVertexFloat3 postion;
		lh_color color;
		TextureUV uv;
		float _rhw = 1.0f;
		VertexColor operator= (const VertexColor &v) {
			postion = v.postion;
			color = v.color;
			uv = v.uv;
			_rhw = v._rhw;
			return *this;
		}
		VertexColor() { }
		VertexColor(LhVertexFloat3 v, lh_color c, TextureUV texture = TextureUV(), float rhw = 1.0) :
			postion(v), color(c), uv(texture), _rhw(rhw) {};
		VertexColor(LhVertexFloat3 v, TextureUV texture = TextureUV(), float rhw = 1.0) :
			postion(v), uv(texture), _rhw(rhw) {};

		void rhw() {
			uv.u *= _rhw;
			uv.v *= _rhw;
			color.red *= _rhw;
			color.green *= _rhw;
			color.blue *= _rhw;
			color.alph *= _rhw;
		}
		VertexColor& operator+=(const VertexColor &v) {
			postion = postion + v.postion;
			color += v.color;
			uv.u += v.uv.u;
			uv.v += v.uv.v;
			_rhw += v._rhw;
			return *this;
		}

		VertexColor operator+(const VertexColor &v) {
			VertexColor r(
				postion + v.postion,
				color + v.color,
				uv + v.uv,
				_rhw + v._rhw);
			return r;
		}

		VertexColor operator+(const VertexColor &v) const {
			VertexColor r(
				postion + v.postion,
				color + v.color,
				uv + v.uv,
				_rhw + v._rhw);
			return r;
		}

		VertexColor operator-(const VertexColor &v) {
			VertexColor r(
				postion - v.postion,
				color - v.color,
				uv - v.uv,
				_rhw - v._rhw);
			return r;
		}

		VertexColor operator-(const VertexColor &v) const {
			VertexColor r(
				postion - v.postion,
				color - v.color,
				uv - v.uv,
				_rhw - v._rhw);
			return r;
		}

		VertexColor operator*(const float v) {
			VertexColor r(
				postion * v,
				color * v,
				uv * v,
				_rhw * v);
			return r;
		}
	};

}