#pragma once
#include "..\lhmath\lhmath_common.h"

namespace lh_pipeline {
    template<typename T>
    static  T lerp(T x1, T x2, float t) {
        return x1 + (x2 - x1)*t;
    };

	template <typename T>
	void swap_vaue(T& a, T& b) {
		T temp = a;
		a = b;
		b = temp;
	}
}
