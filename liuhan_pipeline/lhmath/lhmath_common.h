#pragma once

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

        template <typename T>
        lh_color(T rgba) {
            alph = (float)rgba;
            blue = (float)((int)rgba >> 8);
            green = (float)((int)rgba >> 16);
            red = (float)((int)rgba >> 24);
        }

        template <typename T>
        T get_t() {
            return T((int)alph | (int)blue << 8 | (int)green << 16 | (int)red << 24);
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

}