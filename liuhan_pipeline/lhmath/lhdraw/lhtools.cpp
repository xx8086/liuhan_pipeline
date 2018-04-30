#pragma once
#include "lhtools.hpp"

    template<>
    LhVertex<float, 3> lerp<LhVertex<float, 3> >(LhVertex<float, 3> x1, LhVertex<float, 3> x2, float t) {
        return LhVertex<float, 3>(lerp(x1.get_x(), x2.get_x(), t),
            lerp(x1.get_y(), x2.get_y(), t),
            lerp(x1.get_z(), x2.get_z(), t));
    };

    template<>
    lh_color lerp<lh_color>(lh_color x1, lh_color x2, float t) {
        return lh_color(lerp(x1.red, x2.red, t),
            lerp(x1.green, x2.green, t),
            lerp(x1.blue, x2.blue, t),
            lerp(x1.alph, x2.alph, t));
    };

    template<>
    TextureUV lerp<TextureUV>(TextureUV x1, TextureUV x2, float t) {
        return TextureUV(
            lerp(x1.u, x2.u, t),
            lerp(x1.v, x2.v, t));
    };

    template<>
    VertexColor lerp<VertexColor>(VertexColor x1, VertexColor x2, float t) {
        VertexColor r;
        r.postion = lerp(x1.postion, x2.postion, t);
        r.color = lerp(x1.color, x2.color, t);
        r.uv = lerp(x1.uv, x2.uv, t);
        r._rhw = lerp(x1._rhw, x2._rhw, t);
        return r;
    };

}