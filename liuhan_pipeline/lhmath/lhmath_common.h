#pragma once

namespace lh_pipeline {

#define LH_PI (3.1415926f)

#define ToRadianF(degree) (((float)degree * LH_PI) / 180.0f) 
#define TDegreeF(radian) (((float)radian * 180.0f) / LH_PI)

}