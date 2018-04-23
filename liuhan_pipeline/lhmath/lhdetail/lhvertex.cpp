#include "lhvertex.h"

namespace lh_pipeline {
    //单位化
    bool normalize(LhVertex<float, 3>& vf3) {
        float z = vf3.get_z();
        if (z > 0.99999f &&
            1.00001f > z) {
            return true;
        }

        const float length = sqrtf(vf3.get_x() * vf3.get_x() + vf3.get_y() * vf3.get_y() + vf3.get_z() * vf3.get_z());
        if (length > 0.00001) {
            vf3.set_x(vf3.get_x() / length);
            vf3.set_y(vf3.get_y() / length);
            vf3.set_z(vf3.get_z() / length);
        }
        else {
            return false;
        }
        return true;
    }

    //点乘
    inline float dot(const LhVertex<float, 3>& left, const LhVertex<float, 3>& right) {
        return left.get_x() * right.get_x() +
            left.get_y() * right.get_y() +
            left.get_z() * right.get_z();
    }
    
}