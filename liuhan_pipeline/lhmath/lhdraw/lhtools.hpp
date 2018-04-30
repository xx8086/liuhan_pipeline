#pragma once
#include "..\lhmath_common.h"

namespace lh_pipeline {
    template<typename T>
    T lerp(T x1, T x2, float t) {
        return x1 + (x2 - x1)*t;
    };
}