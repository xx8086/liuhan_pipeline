#pragma once
#include <cmath>

//namespace lh_pipeline {
#define EXTENDED_IMPLEMENTATION_SPECIALIZATION_LHVERTEX

//float
#define OperatorValueSpecializationF2 \
public:\
float get_x() const {return _datas[0];}\
void set_x(float x){_datas[0]= x;}\
float get_y() const{return _datas[1];}\
void set_y(float y){_datas[1]= y;}

#define OperatorValueSpecializationF3 \
OperatorValueSpecializationF2 \
float get_z() const {return _datas[2];}\
void set_z(float z){_datas[2]= z;}

#define OperatorValueSpecializationF4 \
OperatorValueSpecializationF3 \
float get_w() const {return _datas[3];}\
void set_w(float w){_datas[3]= w;}

#define EndSpecializationF2 \
public:\
LhVertex(float x, float y) {\
_datas[0] = x;\
_datas[1] = y;\
}\
};

#define EndSpecializationF3 \
public:\
LhVertex(float x, float y, float z) {\
_datas[0] = x;\
_datas[1] = y;\
_datas[2] = z;\
}\
LhVertex<float, 3> operator-(){\
return LhVertex<float, 3>(-_datas[0], -_datas[1], -_datas[2]);\
}\
};

#define EndSpecializationF4 \
public:\
LhVertex(float x, float y, float z, float w) {\
_datas[0] = x;\
_datas[1] = y;\
_datas[2] = z;\
_datas[3] = w;\
}\
};

#define SpecializationFloat(n) \
template<> \
class LhVertex<float, n> {\
public:\
    unsigned int _counts = n;\
    float _datas[n];\
public:\
    LhVertex() {\
       for(unsigned int i = 0; i < _counts; i++){\
            _datas[i] = 0.0;\
       }\
    }\
OperatorValueSpecializationF##n \
EndSpecializationF##n


//int
#define OperatorValueSpecializationI2 \
public:\
int get_x() const {return _datas[0];}\
void set_x(int x){_datas[0]= x;}\
int get_y() const {return _datas[1];}\
void set_y(int y){_datas[1]= y;}

#define OperatorValueSpecializationI3 \
OperatorValueSpecializationI2 \
int get_z() const {return _datas[2];}\
void set_z(int z){_datas[2]= z;}

#define OperatorValueSpecializationI4 \
OperatorValueSpecializationI3 \
int get_w() const {return _datas[3];}\
void set_w(int w){_datas[3]= w;}

#define EndSpecializationI2 \
public:\
LhVertex(int a, int b) {\
_datas[0] = a;\
_datas[1] = b;\
}\
};

#define EndSpecializationI3 \
public:\
LhVertex(int a, int b, int c) {\
_datas[0] = a;\
_datas[1] = b;\
_datas[2] = c;\
}\
};

#define EndSpecializationI4 \
public:\
LhVertex(int a, int b, int c, int w) {\
_datas[0] = a;\
_datas[1] = b;\
_datas[2] = c;\
_datas[3] = w;\
}\
};

#define SpecializationInt(n) \
template<> \
class LhVertex<int, n> {\
public:\
    unsigned int _counts = n;\
    int _datas[n];\
public:\
    LhVertex() {\
       for(unsigned int i = 0; i < _counts; i++){\
            _datas[i] = 0;\
       }\
    }\
OperatorValueSpecializationI##n \
EndSpecializationI##n

//}