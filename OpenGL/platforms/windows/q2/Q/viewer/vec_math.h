#ifndef __VEC_MATH_H__
#define __VEC_MATH_H__

#include <math.h>

class mat2x3;
class mat3x2;
class mat4x2;
class mat2x4;
class mat4x3;
class mat3x4;
class vec3;
class mat3;
mat3x2 transpose(const mat2x3 &mat);
mat4x2 transpose(const mat2x4 &mat);
mat4x3 transpose(const mat3x4 &mat);
mat3 outerproduct(const vec3 &a, const vec3 &b);
mat3 skew(const vec3 &a);

class vec2 {
    public:
        float   v[2];

        vec2(float p[2])                                        { v[0] = p[0]; v[1] = p[1]; }
        vec2(float x = 0, float y = 0)                          { v[0] = x; v[1] = y; }
        friend vec2 operator-(const vec2 &a, const vec2 &b)     { return vec2(a.v[0] - b.v[0], a.v[1] - b.v[1]); }
        friend vec2 operator+(const vec2 &a, const vec2 &b)     { return vec2(a.v[0] + b.v[0], a.v[1] + b.v[1]); }
        friend vec2 operator-(const vec2 &a, const float b)     { return vec2(a.v[0] - b, a.v[1] - b); }
        friend vec2 operator+(const vec2 &a, const float b)     { return vec2(a.v[0] + b, a.v[1] + b); }
        friend vec2 operator-(const float a, const vec2 &b)     { return vec2(a - b.v[0], a - b.v[1]); }
        friend vec2 operator+(const float a, const vec2 &b)     { return vec2(a + b.v[0], a + b.v[1]); }
        friend bool operator==(const vec2 &a, const vec2 &b)    { return (a.v[0] == b.v[0]) && (a.v[1] == b.v[1]); }
        friend vec2 operator*(const vec2 &a, const vec2 &b)     { return vec2(a.v[0] * b.v[0], a.v[1] * b.v[1]); }
        friend vec2 operator/(const vec2 &a, const vec2 &b)     { return vec2(a.v[0] / b.v[0], a.v[1] / b.v[1]); }
        friend vec2 operator*(const vec2 &a, const float b)     { return vec2(a.v[0] * b, a.v[1] * b); }
        friend vec2 operator/(const vec2 &a, const float b)     { return vec2(a.v[0] / b, a.v[1] / b); }
        friend vec2 operator*(const float a, const vec2 &b)     { return vec2(a * b.v[0], a * b.v[1]); }
        friend vec2 operator/(const float a, const vec2 &b)     { return vec2(a / b.v[0], a / b.v[1]); }
        void operator -=(const vec2 &a)                         { v[0] -= a.v[0]; v[1] -= a.v[1]; }
        void operator +=(const vec2 &a)                         { v[0] += a.v[0]; v[1] += a.v[1]; }
        void operator *=(const float a)                         { v[0] *= a; v[1] *= a; }
        void operator /=(const float a)                         { v[0] /= a; v[1] /= a; }
        friend float dot(const vec2 &a, const vec2 &b)          { return a.v[0]*b.v[0] + a.v[1]*b.v[1]; }
        friend float length(const vec2 &a)                      { return sqrtf(dot(a,a)); }
        friend float distance(const vec2 &a, const vec2 &b)     { return length(a - b); }
        friend vec2 normalize(const vec2 &a)                    { float len = length(a); return a/len; }
        friend vec2 refract(const vec2 &I, const vec2 &N, float eta){
                                                                    float k = 1 - eta * eta *(1 - dot(N,I) * dot(N,I));
                                                                    if(k < 0.0){
                                                                        return vec2(0.0);
                                                                    }
                                                                    else{
                                                                        return (eta * I - (eta * dot(N,I) + sqrt(k)) * N);
                                                                    }
                                                                }
        friend vec2 faceforward(vec2 N, vec2 I, vec2 Nref)      {
                                                                    if(dot(Nref,I) < 0){
                                                                        return (N);                                                                   
                                                                    }
                                                                    else{
                                                                        return (-1 * N);
                                                                    }
                                                                }
        operator const float* ()                                { return v; }
};

class vec3 {
    public:
        float   v[3];
        vec3(vec2 p, float f)                                   { v[0] = p.v[0]; v[1] = p.v[1]; v[2] = f;}
        vec3(float p[3])                                        { v[0] = p[0]; v[1] = p[1]; v[2] = p[2]; }
        vec3(float x = 0, float y = 0, float z = 0)             { v[0] = x; v[1] = y; v[2] = z; }
        friend vec3 operator-(const vec3 &a, const vec3 &b)     { return vec3(a.v[0] - b.v[0], a.v[1] - b.v[1], a.v[2] - b.v[2]); }
        friend vec3 operator+(const vec3 &a, const vec3 &b)     { return vec3(a.v[0] + b.v[0], a.v[1] + b.v[1], a.v[2] + b.v[2]); }
        friend vec3 operator-(const vec3 &a, const float b)     { return vec3(a.v[0] - b, a.v[1] - b, a.v[2] - b); }
        friend vec3 operator+(const vec3 &a, const float b)     { return vec3(a.v[0] + b, a.v[1] + b, a.v[2] + b); }
        friend vec3 operator-(const float a, const vec3 &b)     { return vec3(a - b.v[0], a - b.v[1], a - b.v[2]); }
        friend vec3 operator+(const float a, const vec3 &b)     { return vec3(a + b.v[0], a + b.v[1], a + b.v[2]); }
        friend bool operator==(const vec3 &a, const vec3 &b)    { return (a.v[0] == b.v[0]) && (a.v[1] == b.v[1]) && (a.v[2] == b.v[2]); }
        friend vec3 operator*(const vec3 &a, const vec3 &b)     { return vec3(a.v[0] * b.v[0], a.v[1] * b.v[1], a.v[2] * b.v[2]); }
        friend vec3 operator/(const vec3 &a, const vec3 &b)     { return vec3(a.v[0] / b.v[0], a.v[1] / b.v[1], a.v[2] / b.v[2]); }
        friend vec3 operator*(const vec3 &a, const float b)     { return vec3(a.v[0] * b, a.v[1] * b, a.v[2] * b); }
        friend vec3 operator/(const vec3 &a, const float b)     { return vec3(a.v[0] / b, a.v[1] / b, a.v[2] / b); }
        friend vec3 operator*(const float a, const vec3 &b)     { return vec3(a * b.v[0], a * b.v[1], a * b.v[2]); }
        friend vec3 operator/(const float a, const vec3 &b)     { return vec3(a / b.v[0], a / b.v[1], a / b.v[2]); }
        void operator -=(const vec3 &a)                         { v[0] -= a.v[0]; v[1] -= a.v[1]; v[2] -= a.v[2]; }
        void operator +=(const vec3 &a)                         { v[0] += a.v[0]; v[1] += a.v[1]; v[2] += a.v[2]; }
        void operator *=(const float a)                         { v[0] *= a; v[1] *= a; v[2] *= a; }
        void operator /=(const float a)                         { v[0] /= a; v[1] /= a; v[2] /= a; }
        float &operator[](int index)                            { return v[index]; }
        float operator[](int index) const                       { return v[index]; }
        friend float dot(const vec3 &a, const vec3 &b)          { return a.v[0]*b.v[0] + a.v[1]*b.v[1] + a.v[2]*b.v[2]; }
        friend float length(const vec3 &a)                      { return sqrtf(dot(a,a)); }
        friend float distance(const vec3 &a, const vec3 &b)     { return length(a - b); }
        friend vec3 normalize(const vec3 &a)                    { float len = length(a); return vec3(a.v[0]/len, a.v[1]/len, a.v[2]/len); }
        friend vec3 cross(const vec3 &a, const vec3 &b)         { return vec3(  a.v[1]*b.v[2]-a.v[2]*b.v[1],
                                                                                a.v[2]*b.v[0]-a.v[0]*b.v[2],
                                                                                a.v[0]*b.v[1]-a.v[1]*b.v[0] );  }
        friend vec3 reflect(const vec3 &I, const vec3 &N)       { return I - 2*dot(N,I) * N; }
        friend vec3 refract(const vec3 &I, const vec3 &N, float eta){
                                                                    float k = 1 - eta * eta *(1 - dot(N,I) * dot(N,I));
                                                                    if(k < 0.0){
                                                                        return vec3(0.0);
                                                                    }
                                                                    else{
                                                                        return (eta * I - (eta * dot(N,I) + sqrt(k)) * N);
                                                                    }
                                                                }
        friend vec3 faceforward(vec3 N, vec3 I, vec3 Nref)      {
                                                                    if(dot(Nref,I)<0){
                                                                        return (N);                                                                   
                                                                    }
                                                                    else{
                                                                        return (-1 * N);
                                                                    }
                                                                }
        operator float *()                                      { return v; }
};

class vec4 {
    public:
        float   v[4];
        vec4(const float *p)                                    { v[0] = p[0]; v[1] = p[1]; v[2] = p[2]; v[3] = p[3]; };
        vec4(vec3 v3, float w = 0)                              { v[0] = v3[0]; v[1] = v3[1]; v[2] = v3[2]; v[3] = w; }
        vec4(float x = 0, float y = 0, float z = 0, float w = 0){ v[0] = x; v[1] = y; v[2] = z; v[3] = w; }

        friend vec4 operator-(const vec4 &a, const vec4 &b)     { return vec4(a.v[0] - b.v[0], a.v[1] - b.v[1], a.v[2] - b.v[2], a.v[3] - b.v[3]); }
        friend vec4 operator+(const vec4 &a, const vec4 &b)     { return vec4(a.v[0] + b.v[0], a.v[1] + b.v[1], a.v[2] + b.v[2], a.v[3] + b.v[3]); }

        friend vec4 operator-(const vec4 &a, const float b)     { return vec4(a.v[0] - b, a.v[1] - b, a.v[2] - b, a.v[3] - b); }
        friend vec4 operator+(const vec4 &a, const float b)     { return vec4(a.v[0] + b, a.v[1] + b, a.v[2] + b, a.v[3] + b); }
        friend vec4 operator-(const float a, const vec4 &b)     { return vec4(a - b.v[0], a - b.v[1], a - b.v[2], a - b.v[3]); }
        friend vec4 operator+(const float a, const vec4 &b)     { return vec4(a + b.v[0], a + b.v[1], a + b.v[2], a + b.v[3]); }

        friend bool operator==(const vec4 &a, const vec4 &b)    { return (a.v[0] == b.v[0]) && (a.v[1] == b.v[1]) && (a.v[2] == b.v[2]) && (a.v[3] == b.v[3]); }

        friend vec4 operator*(const vec4 &a, const vec4 &b)     { return vec4(a.v[0] * b.v[0], a.v[1] * b.v[1], a.v[2] * b.v[2], a.v[3] * b.v[3]); }
        friend vec4 operator/(const vec4 &a, const vec4 &b)     { return vec4(a.v[0] / b.v[0], a.v[1] / b.v[1], a.v[2] / b.v[2], a.v[3] / b.v[3]); }

        friend vec4 operator*(const vec4 &a, const float b)     { return vec4(a.v[0] * b, a.v[1] * b, a.v[2] * b, a.v[3] * b); }
        friend vec4 operator/(const vec4 &a, const float b)     { return vec4(a.v[0] / b, a.v[1] / b, a.v[2] / b, a.v[3] / b); }
        friend vec4 operator*(const float a, const vec4 &b)     { return vec4(a * b.v[0], a * b.v[1], a * b.v[2], a * b.v[3]); }
        friend vec4 operator/(const float a, const vec4 &b)     { return vec4(a / b.v[0], a / b.v[1], a / b.v[2], a / b.v[3]); }

        void operator -=(const vec4 &a)                         { v[0] -= a.v[0]; v[1] -= a.v[1]; v[2] -= a.v[2]; v[3] -= a.v[3]; }
        void operator +=(const vec4 &a)                         { v[0] += a.v[0]; v[1] += a.v[1]; v[2] += a.v[2]; v[3] += a.v[3]; }
        void operator *=(const float a)                         { v[0] *= a; v[1] *= a; v[2] *= a; v[3] *= a; }
        void operator /=(const float a)                         { v[0] /= a; v[1] /= a; v[2] /= a; v[3] /= a; }

        float &operator[](int index)                            { return v[index]; }
        float operator[](int index) const                       { return v[index]; }

        friend float dot(const vec4 &a, const vec4 &b)          { return a.v[0]*b.v[0] + a.v[1]*b.v[1] + a.v[2]*b.v[2] + a.v[3]*b.v[3]; }
        friend float length(const vec4 &a)                      { return sqrtf(dot(a,a)); }
        friend float distance(const vec4 &a, const vec4 &b)     { return length(a - b); }
        friend vec4 normalize(const vec4 &a)                    { return a / length(a); }
        friend vec4 reflect(const vec4 &I, const vec4 &N)       { return I - 2*dot(N,I) * N; }

        friend vec4 refract(const vec4 &I, const vec4 &N, float eta){
                                                                    float k = 1 - eta * eta *(1 - dot(N,I) * dot(N,I));
                                                                    if(k < 0.0){
                                                                        return vec4(0.0);
                                                                    }
                                                                    else{
                                                                        return (eta * I - (eta * dot(N,I) + sqrt(k)) * N);
                                                                    }
                                                                }
        friend vec4 faceforward(vec4 N, vec4 I, vec4 Nref)      {
                                                                    if(dot(Nref,I)<0){
                                                                        return (N);                                                                   
                                                                    }
                                                                    else{
                                                                        return (-1 * N);
                                                                    }
                                                                }
        operator float *()                                      { return v; }
};

class mat2 {
    public:
        float m[4];

        mat2(float diag=0)                                      { 
                                                                   m[0]=diag;    m[1]=0; 
                                                                   m[2]=0;       m[3]=diag;
                                                                }
        mat2(float c1r1,float c1r2,
             float c2r1,float c2r2)                             {       
                                                                   m[0]=c1r1; m[1]=c1r2;
                                                                   m[2]=c2r1; m[3]=c2r2;
                                                                   
                                                                }
        friend mat2 operator-(const mat2 &a, const mat2 &b)     { return mat2(a.m[0]-b.m[0],a.m[1]-b.m[1], a.m[2]-b.m[2],a.m[3]-b.m[3]); }
        friend mat2 operator+(const mat2 &a, const mat2 &b)     { return mat2(a.m[0]+b.m[0],a.m[1]+b.m[1], a.m[2]+b.m[2],a.m[3]+b.m[3]); }
        friend mat2 operator*(const float a, const mat2 &b)     { return mat2(a*b.m[0],a*b.m[1], a*b.m[2],a*b.m[3]); }
        friend vec2 operator*(const vec2 &a, const mat2 &b)     { return vec2(a.v[0]*b.m[0]+a.v[1]*b.m[1], a.v[0]*b.m[2]+a.v[1]*b.m[3]); }
        friend vec2 operator*(const mat2 &b, const vec2 &a)     { return vec2(a.v[0]*b.m[0]+a.v[1]*b.m[2], a.v[0]*b.m[1]+a.v[1]*b.m[3]); }
        friend mat2 operator*(const mat2 &a, const mat2 &b)     { 
                                                                  return mat2( a.m[0] * b.m[0] + a.m[2] * b.m[1], a.m[1] * b.m[0] + a.m[3] * b.m[1],
                                                                               a.m[0] * b.m[2] + a.m[2] * b.m[3], a.m[1] * b.m[2] + a.m[3] * b.m[3] );
                                                                }

        friend mat2 outerproduct(const vec2 &a, const vec2 &b)  { return mat2(a.v[0]*b.v[0],a.v[1]*b.v[0],
                                                                              a.v[0]*b.v[1],a.v[1]*b.v[1]);
                                                                }
        friend mat2 transpose(const mat2 &mat)                  { return mat2(mat.m[0], mat.m[2],
                                                                              mat.m[1], mat.m[3]);
                                                                }
        friend mat2 inverse(const mat2 &mat)                    {
                                                                    float det = mat.m[0]*mat.m[3] - mat.m[2]*mat.m[1];
                                                                    if (det == 0)
                                                                        return mat2(0);
                                                                    return mat2( mat.m[3]/det, -mat.m[1]/det,
                                                                                -mat.m[2]/det,  mat.m[0]/det );

                                                                }

        friend float determinant(const mat2 &mat)               {   
                                                                    return ( mat.m[0] * mat.m[3] - mat.m[1] * mat.m[2]);
                                                                }
        operator float *()                                      { return m; }
};




class mat3 {
    public:
        float m[9];
        mat3(float diag=0)                                      { 
                                                                   m[0]=diag;  m[3]=0;     m[6]=0; 
                                                                   m[1]=0;     m[4]=diag;  m[7]=0; 
                                                                   m[2]=0;     m[5]=0;     m[8]=diag;
                                                                }

        mat3(float p[9])                                        { 
                                                                   m[0]=p[0];  m[1]=p[1];  m[2]=p[2];
                                                                   m[3]=p[3];  m[4]=p[4];  m[5]=p[5];
                                                                   m[6]=p[6];  m[7]=p[7];  m[8]=p[8];
                                                                }
        mat3(vec3 c1, vec3 c2, vec3 c3)                         { 
                                                                   m[0]=c1.v[0];  m[1]=c1.v[1];  m[2]=c1.v[2];
                                                                   m[3]=c2.v[0];  m[4]=c2.v[1];  m[5]=c2.v[2];
                                                                   m[6]=c3.v[0];  m[7]=c3.v[1];  m[8]=c3.v[2];
                                                                }
        mat3(float c1r1,float c1r2,float c1r3,
             float c2r1,float c2r2,float c2r3,
             float c3r1,float c3r2,float c3r3)                  { m[0]=c1r1; m[1]=c1r2; m[2]=c1r3;
                                                                  m[3]=c2r1; m[4]=c2r2; m[5]=c2r3;
                                                                  m[6]=c3r1; m[7]=c3r2; m[8]=c3r3;
                                                                }

        friend mat3 operator-(const mat3 &a, const mat3 &b)     { return mat3(a.m[0]-b.m[0],a.m[1]-b.m[1],a.m[2]-b.m[2], a.m[3]-b.m[3],a.m[4]-b.m[4],a.m[5]-b.m[5], a.m[6]-b.m[6],a.m[7]-b.m[7],a.m[8]-b.m[8]); }
        friend mat3 operator+(const mat3 &a, const mat3 &b)     { return mat3(a.m[0]+b.m[0],a.m[1]+b.m[1],a.m[2]+b.m[2], a.m[3]+b.m[3],a.m[4]+b.m[4],a.m[5]+b.m[5], a.m[6]+b.m[6],a.m[7]+b.m[7],a.m[8]+b.m[8]); }
        friend mat3 operator*(const float a, const mat3 &b)     { return mat3(a*b.m[0],a*b.m[1],a*b.m[2], a*b.m[3],a*b.m[4],a*b.m[5], a*b.m[6],a*b.m[7],a*b.m[8]); }
        friend vec3 operator*(const vec3 &a, const mat3 &b)     { return vec3(a.v[0]*b.m[0]+a.v[1]*b.m[1]+a.v[2]*b.m[2], a.v[0]*b.m[3]+a.v[1]*b.m[4]+a.v[2]*b.m[5], a.v[0]*b.m[6]+a.v[1]*b.m[7]+a.v[2]*b.m[8]); }
        friend vec3 operator*(const mat3 &b, const vec3 &a)     { return vec3(a.v[0]*b.m[0]+a.v[1]*b.m[3]+a.v[2]*b.m[6], a.v[0]*b.m[1]+a.v[1]*b.m[4]+a.v[2]*b.m[7], a.v[0]*b.m[2]+a.v[1]*b.m[5]+a.v[2]*b.m[8]); }
        friend mat3 operator*(const mat3 &a, const mat3 &b)     { //3x3 . 3x3 
                                                                  return mat3( a.m[0] * b.m[0] + a.m[3] * b.m[1] + a.m[6] * b.m[2],
                                                                               a.m[1] * b.m[0] + a.m[4] * b.m[1] + a.m[7] * b.m[2],
                                                                               a.m[2] * b.m[0] + a.m[5] * b.m[1] + a.m[8] * b.m[2],

                                                                               a.m[0] * b.m[3] + a.m[3] * b.m[4] + a.m[6] * b.m[5],
                                                                               a.m[1] * b.m[3] + a.m[4] * b.m[4] + a.m[7] * b.m[5],
                                                                               a.m[2] * b.m[3] + a.m[5] * b.m[4] + a.m[8] * b.m[5],

                                                                               a.m[0] * b.m[6] + a.m[3] * b.m[7] + a.m[6] * b.m[8],
                                                                               a.m[1] * b.m[6] + a.m[4] * b.m[7] + a.m[7] * b.m[8],
                                                                               a.m[2] * b.m[6] + a.m[5] * b.m[7] + a.m[8] * b.m[8] );
                                                                }
        friend mat3 outerproduct(const vec3 &a, const vec3 &b)  { return mat3(a.v[0]*b.v[0],a.v[1]*b.v[0],a.v[2]*b.v[0], a.v[0]*b.v[1],a.v[1]*b.v[1],a.v[2]*b.v[1], a.v[0]*b.v[2],a.v[1]*b.v[2],a.v[2]*b.v[2]); }
        friend mat3 skew(const vec3 &a)                         { return mat3(0, a.v[2], -a.v[1], -a.v[2], 0, a.v[0], a.v[1], -a.v[0], 0);}
        friend mat3 transpose(const mat3 &mat)                  { return mat3(mat.m[0], mat.m[3], mat.m[6],
                                                                              mat.m[1], mat.m[4], mat.m[7],
                                                                              mat.m[2], mat.m[5], mat.m[8]);
                                                                }
        friend mat3 inverse(const mat3 &mat)                    {
                                                                    float a = mat.m[0], b = mat.m[3], c = mat.m[6];
                                                                    float d = mat.m[1], e = mat.m[4], f = mat.m[7];
                                                                    float g = mat.m[2], h = mat.m[5], i = mat.m[8];
                                                                    float det = a*(e*i - f*h) - b*(d*i - f*g) + c*(d*h - e*g);
                                                                    if (det == 0.0f)
                                                                        return mat3(0.0f);
                                                                    return mat3( (e*i - f*h)/det, (f*g - d*i)/det, (d*h - e*g)/det,
                                                                                 (c*h - b*i)/det, (a*i - c*g)/det, (b*g - a*h)/det,
                                                                                 (b*f - c*e)/det, (c*d - a*f)/det, (a*e - b*d)/det );
                                                                }

        friend float determinant(const mat3 &mat)               { 
                                                                  return( mat.m[0] * mat.m[4] * mat.m[8] + mat.m[1] * mat.m[5] * mat.m[6] + mat.m[2] * mat.m[3] * mat.m[7] -
                                                                          mat.m[2] * mat.m[4] * mat.m[6] - mat.m[1] * mat.m[3] * mat.m[8] - mat.m[0] * mat.m[5] * mat.m[7]   );
                                                                }

        operator float *()                                      { return m; }
};



class mat4 {
    public:
        float m[16];
        mat4(float diag=0)                                      {   m[0]=diag;  m[1]=0;     m[2]=0;     m[3]=0;
                                                                    m[4]=0;     m[5]=diag;  m[6]=0;     m[7]=0;
                                                                    m[8]=0;     m[9]=0;     m[10]=diag; m[11]=0;
                                                                    m[12]=0;    m[13]=0;    m[14]=0;    m[15]=diag;
                                                                }
        mat4(vec4 c1, vec4 c2, vec4 c3, vec4 c4)                {   m[0]=c1.v[0];   m[1]=c1.v[1];   m[2]=c1.v[2];   m[3]=c1.v[3];
                                                                    m[4]=c2.v[0];   m[5]=c2.v[1];   m[6]=c2.v[2];   m[7]=c2.v[3];
                                                                    m[8]=c3.v[0];   m[9]=c3.v[1];   m[10]=c3.v[2];  m[11]=c3.v[3];
                                                                    m[12]=c4.v[0];  m[13]=c4.v[1];  m[14]=c4.v[2];  m[15]=c4.v[3];
                                                                }
        mat4(const float arry[16])                              {   m[0]=arry[0];   m[1]=arry[1];   m[2]=arry[2];   m[3]=arry[3];
                                                                    m[4]=arry[4];   m[5]=arry[5];   m[6]=arry[6];   m[7]=arry[7];
                                                                    m[8]=arry[8];   m[9]=arry[9];   m[10]=arry[10]; m[11]=arry[11];
                                                                    m[12]=arry[12]; m[13]=arry[13]; m[14]=arry[14]; m[15]=arry[15];
                                                                }
        mat4(float c1r1,float c1r2,float c1r3,float c1r4,
             float c2r1,float c2r2,float c2r3,float c2r4,
             float c3r1,float c3r2,float c3r3,float c3r4,
             float c4r1,float c4r2,float c4r3,float c4r4)       {   m[0]=c1r1;  m[1]=c1r2;  m[2]=c1r3;  m[3]=c1r4;
                                                                    m[4]=c2r1;  m[5]=c2r2;  m[6]=c2r3;  m[7]=c2r4;
                                                                    m[8]=c3r1;  m[9]=c3r2;  m[10]=c3r3; m[11]=c3r4;
                                                                    m[12]=c4r1; m[13]=c4r2; m[14]=c4r3; m[15]=c4r4;
                                                                }
        float& operator[]( int index )                          { return m[index%16]; }

        friend mat4 operator-(const mat4 &a, const mat4 &b)     { return mat4(  a.m[0]-b.m[0],  a.m[1]-b.m[1],  a.m[2]-b.m[2],  a.m[3]-b.m[3],
                                                                                a.m[4]-b.m[4],  a.m[5]-b.m[5],  a.m[6]-b.m[6],  a.m[7]-b.m[7],
                                                                                a.m[8]-b.m[8],  a.m[9]-b.m[9],  a.m[10]-b.m[10],a.m[11]-b.m[11],
                                                                                a.m[12]-b.m[12],a.m[13]-b.m[13],a.m[14]-b.m[14],a.m[15]-b.m[15] );
                                                                }
        friend mat4 operator+(const mat4 &a, const mat4 &b)     { return mat4(  a.m[0]+b.m[0],  a.m[1]+b.m[1],  a.m[2]+b.m[2],  a.m[3]+b.m[3],
                                                                                a.m[4]+b.m[4],  a.m[5]+b.m[5],  a.m[6]+b.m[6],  a.m[7]+b.m[7],
                                                                                a.m[8]+b.m[8],  a.m[9]+b.m[9],  a.m[10]+b.m[10],a.m[11]+b.m[11],
                                                                                a.m[12]+b.m[12],a.m[13]+b.m[13],a.m[14]+b.m[14],a.m[15]+b.m[15] );
                                                                }
        friend mat4 operator*(const float a, const mat4 &b)     { return mat4(  a*b.m[0],   a*b.m[1],   a*b.m[2],   a*b.m[3],
                                                                                a*b.m[4],   a*b.m[5],   a*b.m[6],   a*b.m[7],
                                                                                a*b.m[8],   a*b.m[9],   a*b.m[10],  a*b.m[11],
                                                                                a*b.m[12],  a*b.m[13],  a*b.m[14],  a*b.m[15] );
                                                                }
        friend vec4 operator*(const vec4 &a, const mat4 &b)     { return vec4(  a.v[0]*b.m[0] + a.v[1]*b.m[1] + a.v[2]*b.m[2] + a.v[3]*b.m[3],
                                                                                a.v[0]*b.m[4] + a.v[1]*b.m[5] + a.v[2]*b.m[6] + a.v[3]*b.m[7],
                                                                                a.v[0]*b.m[8] + a.v[1]*b.m[9] + a.v[2]*b.m[10]+ a.v[3]*b.m[11],
                                                                                a.v[0]*b.m[12]+ a.v[1]*b.m[13]+ a.v[2]*b.m[14]+ a.v[3]*b.m[15] );
                                                                }
        friend vec4 operator*(const mat4 &b, const vec4 &a)     { return vec4(  a.v[0]*b.m[0] + a.v[1]*b.m[4] + a.v[2]*b.m[8] + a.v[3]*b.m[12],
                                                                                a.v[0]*b.m[1] + a.v[1]*b.m[5] + a.v[2]*b.m[9] + a.v[3]*b.m[13],
                                                                                a.v[0]*b.m[2] + a.v[1]*b.m[6] + a.v[2]*b.m[10]+ a.v[3]*b.m[14],
                                                                                a.v[0]*b.m[3] + a.v[1]*b.m[7] + a.v[2]*b.m[11]+ a.v[3]*b.m[15] );
                                                                }

        friend mat4 operator*(const mat4 &a, const mat4 &b)     { // 4x4 . 4x4
                                                                  return mat4( a.m[0] * b.m[0] + a.m[4] * b.m[1] + a.m[8] * b.m[2] + a.m[12]* b.m[3],
                                                                               a.m[1] * b.m[0] + a.m[5] * b.m[1] + a.m[9] * b.m[2] + a.m[13]* b.m[3],
                                                                               a.m[2] * b.m[0] + a.m[6] * b.m[1] + a.m[10]* b.m[2] + a.m[14]* b.m[3],
                                                                               a.m[3] * b.m[0] + a.m[7] * b.m[1] + a.m[11]* b.m[2] + a.m[15]* b.m[3],

                                                                               a.m[0] * b.m[4] + a.m[4] * b.m[5] + a.m[8] * b.m[6] + a.m[12]* b.m[7],
                                                                               a.m[1] * b.m[4] + a.m[5] * b.m[5] + a.m[9] * b.m[6] + a.m[13]* b.m[7],
                                                                               a.m[2] * b.m[4] + a.m[6] * b.m[5] + a.m[10]* b.m[6] + a.m[14]* b.m[7],
                                                                               a.m[3] * b.m[4] + a.m[7] * b.m[5] + a.m[11]* b.m[6] + a.m[15]* b.m[7],

                                                                               a.m[0] * b.m[8] + a.m[4] * b.m[9] + a.m[8] * b.m[10] + a.m[12]* b.m[11],
                                                                               a.m[1] * b.m[8] + a.m[5] * b.m[9] + a.m[9] * b.m[10] + a.m[13]* b.m[11],
                                                                               a.m[2] * b.m[8] + a.m[6] * b.m[9] + a.m[10]* b.m[10] + a.m[14]* b.m[11],
                                                                               a.m[3] * b.m[8] + a.m[7] * b.m[9] + a.m[11]* b.m[10] + a.m[15]* b.m[11],

                                                                               a.m[0] * b.m[12]+ a.m[4] * b.m[13]+ a.m[8] * b.m[14] + a.m[12]* b.m[15],
                                                                               a.m[1] * b.m[12]+ a.m[5] * b.m[13]+ a.m[9] * b.m[14] + a.m[13]* b.m[15],
                                                                               a.m[2] * b.m[12]+ a.m[6] * b.m[13]+ a.m[10]* b.m[14] + a.m[14]* b.m[15],
                                                                               a.m[3] * b.m[12]+ a.m[7] * b.m[13]+ a.m[11]* b.m[14] + a.m[15]* b.m[15] );
                                                                }

        friend mat4 axisAngle(float angle, const vec3 &axis)    {
                                                                    float c = cosf(angle), s = sinf(angle);
                                                                    return mat4(
                                                                        axis[0]*axis[0]*(1 - c) + c        , axis[1]*axis[0]*(1 - c) + axis[2]*s, axis[2]*axis[0]*(1 - c) - axis[1]*s, 0,
                                                                        axis[0]*axis[1]*(1 - c) - axis[2]*s, axis[1]*axis[1]*(1 - c) + c        , axis[2]*axis[1]*(1 - c) + axis[0]*s, 0,
                                                                        axis[0]*axis[2]*(1 - c) + axis[1]*s, axis[1]*axis[2]*(1 - c) - axis[0]*s, axis[2]*axis[2]*(1 - c) + c        , 0,
                                                                        0, 0, 0, 1);
                                                                }

        friend mat4 fromQuaternion(const vec4 &q)               {
                                                                    return mat4(
                                                                        1 - 2 * (q[1] * q[1] + q[2] * q[2]),     2 * (q[0] * q[1] + q[2] * q[3]),     2 * (q[0] * q[2] - q[1] * q[3]), 0,
                                                                            2 * (q[0] * q[1] - q[2] * q[3]), 1 - 2 * (q[0] * q[0] + q[2] * q[2]),     2 * (q[1] * q[2] + q[0] * q[3]), 0,
                                                                            2 * (q[0] * q[2] + q[1] * q[3]),     2 * (q[1] * q[2] - q[0] * q[3]), 1 - 2 * (q[0] * q[0] + q[1] * q[1]), 0,
                                                                        0, 0, 0, 1
                                                                        );
                                                                };

        friend mat4 outerproduct(const vec4 &a, const vec4 &b)  {
                                                                  return mat4(  a.v[0]*b.v[0], a.v[1]*b.v[0], a.v[2]*b.v[0], a.v[3]*b.v[0],
                                                                                a.v[0]*b.v[1], a.v[1]*b.v[1], a.v[2]*b.v[1], a.v[3]*b.v[1],
                                                                                a.v[0]*b.v[2], a.v[1]*b.v[2], a.v[2]*b.v[2], a.v[3]*b.v[2],
                                                                                a.v[0]*b.v[3], a.v[1]*b.v[3], a.v[2]*b.v[3], a.v[3]*b.v[3] );
                                                                }

        friend mat4 transpose(const mat4 &mat)                  { 
                                                                  return mat4(mat.m[0], mat.m[4], mat.m[8],  mat.m[12],
                                                                              mat.m[1], mat.m[5], mat.m[9],  mat.m[13],
                                                                              mat.m[2], mat.m[6], mat.m[10], mat.m[14],
                                                                              mat.m[3], mat.m[7], mat.m[11], mat.m[15]);
                                                                }
        friend mat4 inverse(const mat4 &A)                      {   // http://www.cvl.iis.u-tokyo.ac.jp/~miyazaki/tech/teche23.html
                                                                    float a11 = A.m[0];
                                                                    float a12 = A.m[1];
                                                                    float a13 = A.m[2];
                                                                    float a14 = A.m[3];
                                                                    float a21 = A.m[4];
                                                                    float a22 = A.m[5];
                                                                    float a23 = A.m[6];
                                                                    float a24 = A.m[7];
                                                                    float a31 = A.m[8];
                                                                    float a32 = A.m[9];
                                                                    float a33 = A.m[10];
                                                                    float a34 = A.m[11];
                                                                    float a41 = A.m[12];
                                                                    float a42 = A.m[13];
                                                                    float a43 = A.m[14];
                                                                    float a44 = A.m[15];
                                                                    float b11,b12,b13,b14,
                                                                          b21,b22,b23,b24,
                                                                          b31,b32,b33,b34,
                                                                          b41,b42,b43,b44;

                                                                    float detA = a11*a22*a33*a44 + a11*a23*a34*a42 + a11*a24*a32*a43
                                                                               + a12*a21*a34*a43 + a12*a23*a31*a44 + a12*a24*a33*a41
                                                                               + a13*a21*a32*a44 + a13*a22*a34*a41 + a13*a24*a31*a42
                                                                               + a14*a21*a33*a42 + a14*a22*a31*a43 + a14*a23*a32*a41
                                                                               - a11*a22*a34*a43 - a11*a23*a32*a44 - a11*a24*a33*a42
                                                                               - a12*a21*a33*a44 - a12*a23*a34*a41 - a12*a24*a31*a43
                                                                               - a13*a21*a34*a42 - a13*a22*a31*a44 - a13*a24*a32*a41
                                                                               - a14*a21*a32*a43 - a14*a22*a33*a41 - a14*a23*a31*a42;
                                                                    if (detA == 0)
                                                                        return mat4(0.0f);
                                                                    b11 = a22*a33*a44 + a23*a34*a42 + a24*a32*a43 - a22*a34*a43 - a23*a32*a44 - a24*a33*a42;
                                                                    b12 = a12*a34*a43 + a13*a32*a44 + a14*a33*a42 - a12*a33*a44 - a13*a34*a42 - a14*a32*a43;
                                                                    b13 = a12*a23*a44 + a13*a24*a42 + a14*a22*a43 - a12*a24*a43 - a13*a22*a44 - a14*a23*a42;
                                                                    b14 = a12*a24*a33 + a13*a22*a34 + a14*a23*a32 - a12*a23*a34 - a13*a24*a32 - a14*a22*a33;

                                                                    b21 = a21*a34*a43 + a23*a31*a44 + a24*a33*a41 - a21*a33*a44 - a23*a34*a41 - a24*a31*a43;
                                                                    b22 = a11*a33*a44 + a13*a34*a41 + a14*a31*a43 - a11*a34*a43 - a13*a31*a44 - a14*a33*a41;
                                                                    b23 = a11*a24*a43 + a13*a21*a44 + a14*a23*a41 - a11*a23*a44 - a13*a24*a41 - a14*a21*a43;
                                                                    b24 = a11*a23*a34 + a13*a24*a31 + a14*a21*a33 - a11*a24*a33 - a13*a21*a34 - a14*a23*a31;

                                                                    b31 = a21*a32*a44 + a22*a34*a41 + a24*a31*a42 - a21*a34*a42 - a22*a31*a44 - a24*a32*a41;
                                                                    b32 = a11*a34*a42 + a12*a31*a44 + a14*a32*a41 - a11*a32*a44 - a12*a34*a41 - a14*a31*a42;
                                                                    b33 = a11*a22*a44 + a12*a24*a41 + a14*a21*a42 - a11*a24*a42 - a12*a21*a44 - a14*a22*a41;
                                                                    b34 = a11*a24*a32 + a12*a21*a34 + a14*a22*a31 - a11*a22*a34 - a12*a24*a31 - a14*a21*a32;

                                                                    b41 = a21*a33*a42 + a22*a31*a43 + a23*a32*a41 - a21*a32*a43 - a22*a33*a41 - a23*a31*a42;
                                                                    b42 = a11*a32*a43 + a12*a33*a41 + a13*a31*a42 - a11*a33*a42 - a12*a31*a43 - a13*a32*a41;
                                                                    b43 = a11*a23*a42 + a12*a21*a43 + a13*a22*a41 - a11*a22*a43 - a12*a23*a41 - a13*a21*a42;
                                                                    b44 = a11*a22*a33 + a12*a23*a31 + a13*a21*a32 - a11*a23*a32 - a12*a21*a33 - a13*a22*a31;
                                                                    return (1 / detA) * mat4( b11, b12, b13, b14,
                                                                                              b21, b22, b23, b24,
                                                                                              b31, b32, b33, b34,
                                                                                              b41, b42, b43, b44 );
                                                                }
        operator float *()                                      { return m; }
};
class mat2x3 { //2 columns 3 rows
    public:
        float m[6];
        mat2x3(float diag=0)                                    {   m[0]=diag;  m[1]=0;
                                                                    m[2]=0;     m[3]=diag;
                                                                    m[4]=0;     m[5]=0;
                                                                }
        mat2x3(float c1r1,float c1r2,float c1r3,
               float c2r1,float c2r2,float c2r3)                {   m[0]=c1r1;  m[3]=c2r1;
                                                                    m[1]=c1r2;  m[4]=c2r2;
                                                                    m[2]=c1r3;  m[5]=c2r3;
                                                                }
        friend mat2x3 operator-(const mat2x3 &a, const mat2x3 &b){
                                                                    return mat2x3(  a.m[0]-b.m[0],  a.m[1]-b.m[1],
                                                                                    a.m[2]-b.m[2],  a.m[3]-b.m[3],
                                                                                    a.m[4]-b.m[4],  a.m[5]-b.m[5] );
                                                                }
        friend mat2x3 operator+(const mat2x3 &a, const mat2x3 &b){
                                                                    return mat2x3(  a.m[0]+b.m[0],  a.m[1]+b.m[1],
                                                                                    a.m[2]+b.m[2],  a.m[3]+b.m[3],
                                                                                    a.m[4]+b.m[4],  a.m[5]+b.m[5] );
                                                                }
        friend mat2x3 operator*(const float a,const mat2x3 &b)  { 
                                                                   return mat2x3( a * b.m[0], a * b.m[1],
                                                                                  a * b.m[2], a * b.m[3],
                                                                                  a * b.m[4], a * b.m[5] );
                                                                }

        friend vec3 operator*(const mat2x3 &mat, const vec2 &v2){ //3 rows X 2 columns . 2rows X 1 columns 
                                                                   return vec3( mat.m[0] * v2.v[0] + mat.m[3] * v2.v[1],
                                                                                mat.m[1] * v2.v[0] + mat.m[4] * v2.v[1],
                                                                                mat.m[2] * v2.v[0] + mat.m[5] * v2.v[1] );
                                                                }

        friend vec2 operator*(const vec3 &v3, const mat2x3 &mat){ 
                                                                   return vec2( v3.v[0] * mat.m[0] + v3.v[1] * mat.m[1] + v3.v[2] * mat.m[2],
                                                                                v3.v[0] * mat.m[3] + v3.v[1] * mat.m[4] + v3.v[2] * mat.m[5] );
                                                                }
        friend mat2x3 operator*(const mat2x3 &a, const mat2 &b) {//3x2 . 2x2
                                                                   return mat2x3( a.m[0] * b.m[0] + a.m[3] * b.m[1],
                                                                                  a.m[1] * b.m[0] + a.m[4] * b.m[1],
                                                                                  a.m[2] * b.m[0] + a.m[5] * b.m[1],

                                                                                  a.m[0] * b.m[2] + a.m[3] * b.m[3],
                                                                                  a.m[1] * b.m[2] + a.m[4] * b.m[3],
                                                                                  a.m[2] * b.m[2] + a.m[5] * b.m[3] );
                                                                }
        friend mat2x3 operator*(const mat3 &a, const mat2x3 &b) { //3x3 . 3x2 
                                                                  return mat2x3( a.m[0] * b.m[0] + a.m[3] * b.m[1] + a.m[6] * b.m[2], 
                                                                                 a.m[1] * b.m[0] + a.m[4] * b.m[1] + a.m[7] * b.m[2],
                                                                                 a.m[2] * b.m[0] + a.m[5] * b.m[1] + a.m[8] * b.m[2], 

                                                                                 a.m[0] * b.m[3] + a.m[3] * b.m[4] + a.m[6] * b.m[5],
                                                                                 a.m[1] * b.m[3] + a.m[4] * b.m[4] + a.m[7] * b.m[5], 
                                                                                 a.m[2] * b.m[3] + a.m[5] * b.m[4] + a.m[8] * b.m[5] );
                                                                }
        friend mat2x3 outerproduct(const vec2 &c, const vec3 &r){
                                                                   return mat2x3( c.v[0]*r.v[0], c.v[0]*r.v[1], c.v[0]*r.v[2], 
                                                                                  c.v[1]*r.v[0], c.v[1]*r.v[1], c.v[1]*r.v[2] );
                                                                }
        operator float *()                                      { return m; }
};

class mat2x4{ //2 columns 4 rows 
    public:
        float m[8];
        mat2x4(float diag = 0)                                    {
                                                                      m[0]=diag;  m[4]=0;
                                                                      m[1]=0;     m[5]=diag;
                                                                      m[2]=0;     m[6]=0;
                                                                      m[3]=0;     m[7]=0;
                                                                  }

        mat2x4(float c1r1, float c1r2, float c1r3, float c1r4,
               float c2r1, float c2r2, float c2r3, float c2r4)    {
                                                                      m[0]=c1r1; m[1]=c1r2; m[2]=c1r3; m[3]=c1r4;
                                                                      m[4]=c2r1; m[5]=c2r2; m[6]=c2r3; m[7]=c2r4;
                                                                  }

        friend mat2x4 operator+(const mat2x4 &a, const mat2x4 &b) {
                                                                    return mat2x4( a.m[0] + b.m[0], a.m[1]+ b.m[1],
                                                                                   a.m[2] + b.m[2], a.m[3]+ b.m[3],
                                                                                   a.m[4] + b.m[4], a.m[5]+ b.m[5],
                                                                                   a.m[6] + b.m[6], a.m[7]+ b.m[7]);
                                                                  }

        friend mat2x4 operator-(const mat2x4 &a, const mat2x4 &b) {
                                                                    return mat2x4( a.m[0] - b.m[0], a.m[1]- b.m[1],
                                                                                   a.m[2] - b.m[2], a.m[3]- b.m[3],
                                                                                   a.m[4] - b.m[4], a.m[5]- b.m[5],
                                                                                   a.m[6] - b.m[6], a.m[7]- b.m[7]);
                                                                  }

        friend mat2x4 operator*(const float a, const mat2x4 &b)   {
                                                                    return mat2x4( a * b.m[0], a * b.m[1],
                                                                                   a * b.m[2], a * b.m[3],
                                                                                   a * b.m[4], a * b.m[5],
                                                                                   a * b.m[6], a * b.m[7]  );
                                                                  }

        friend vec4 operator*(const mat2x4 &mat, const vec2 &v2)  {
                                                                    return vec4( mat.m[0] * v2.v[0] + mat.m[4] * v2.v[1],
                                                                                 mat.m[1] * v2.v[0] + mat.m[5] * v2.v[1],
                                                                                 mat.m[2] * v2.v[0] + mat.m[6] * v2.v[1],
                                                                                 mat.m[3] * v2.v[0] + mat.m[7] * v2.v[1] );
                                                                  }
        friend vec2 operator*(const vec4 &v4, const mat2x4 &mat)  { 
                                                                    return vec2( v4.v[0] * mat.m[0] + v4.v[1] * mat.m[1] + v4.v[2] * mat.m[2] + v4.v[3] * mat.m[3],
                                                                                 v4.v[0] * mat.m[4] + v4.v[1] * mat.m[5] + v4.v[2] * mat.m[6] + v4.v[3] * mat.m[7] );
                                                                  }
        friend mat2x4 operator*(const mat4 &a, const mat2x4 &b)   { 
                                                                    return mat2x4( a.m[0] * b.m[0] + a.m[4] * b.m[1] + a.m[8] * b.m[2] + a.m[12]* b.m[3], 
                                                                                   a.m[1] * b.m[0] + a.m[5] * b.m[1] + a.m[9] * b.m[2] + a.m[13]* b.m[3],
                                                                                   a.m[2] * b.m[0] + a.m[6] * b.m[1] + a.m[10]* b.m[2] + a.m[14]* b.m[3], 
                                                                                   a.m[3] * b.m[0] + a.m[7] * b.m[1] + a.m[11]* b.m[2] + a.m[15]* b.m[3],

                                                                                   a.m[0] * b.m[4] + a.m[4] * b.m[5] + a.m[8] * b.m[6] + a.m[12]* b.m[7], 
                                                                                   a.m[1] * b.m[4] + a.m[5] * b.m[5] + a.m[9] * b.m[6] + a.m[13]* b.m[7],
                                                                                   a.m[2] * b.m[4] + a.m[6] * b.m[5] + a.m[10]* b.m[6] + a.m[14]* b.m[7], 
                                                                                   a.m[3] * b.m[4] + a.m[7] * b.m[5] + a.m[11]* b.m[6] + a.m[15]* b.m[7] );
                                                                  }

        friend mat2x4 operator*(const mat2x4 &a, const mat2 &b)   { 
                                                                    return mat2x4( a.m[0] * b.m[0] + a.m[4] * b.m[1],
                                                                                   a.m[1] * b.m[0] + a.m[5] * b.m[1],        
                                                                                   a.m[2] * b.m[0] + a.m[6] * b.m[1],
                                                                                   a.m[3] * b.m[0] + a.m[7] * b.m[1],
                                                                                
                                                                                   a.m[0] * b.m[2] + a.m[4] * b.m[3],
                                                                                   a.m[1] * b.m[2] + a.m[5] * b.m[3],                                                                                
                                                                                   a.m[2] * b.m[2] + a.m[6] * b.m[3],
                                                                                   a.m[3] * b.m[2] + a.m[7] * b.m[3] );
                                                                  }
        friend mat2x4 outerproduct(const vec2 &c, const vec4 &r)  { 
                                                                    return mat2x4( c.v[0] * r.v[0], c.v[0] * r.v[1], c.v[0] * r.v[2], c.v[0] * r.v[3],
                                                                                   c.v[1] * r.v[0], c.v[1] * r.v[1], c.v[1] * r.v[2], c.v[1] * r.v[3] );
                                                                  }

        operator float *()                                        { return m; }
};


class mat3x2{ //3 columns 2 rows 
    public:
        float m[6];
        mat3x2(float diag=0)                                      {   m[0]=diag;  m[1]=0;     m[2]=0;
                                                                      m[3]=0;     m[4]=diag;  m[5]=0;
                                                                  }
        mat3x2(float c1r1, float c1r2, 
               float c2r1, float c2r2, 
               float c3r1, float c3r2)                            {   m[0]=c1r1;  m[2]=c2r1;  m[4]=c3r1;
                                                                      m[1]=c1r2;  m[3]=c2r2;  m[5]=c3r2;
                                                                  }

        friend mat3x2 operator+(const mat3x2 &a, const mat3x2 &b) { 
                                                                    return mat3x2( a.m[0] + b.m[0], a.m[1] + b.m[1], a.m[2] + b.m[2],
                                                                                   a.m[3] + b.m[3], a.m[4] + b.m[4], a.m[5] + b.m[5] );
                                                                  }

        friend mat3x2 operator-(const mat3x2 &a, const mat3x2 &b) { 
                                                                    return mat3x2( a.m[0] - b.m[0], a.m[1] - b.m[1], a.m[2] - b.m[2],
                                                                                   a.m[3] - b.m[3], a.m[4] - b.m[4], a.m[5] - b.m[5] );
                                                                  }

        friend mat3x2 operator*(const float a,const mat3x2 &b)    { 
                                                                    return mat3x2( a * b.m[0], a * b.m[1], a * b.m[2],
                                                                                   a * b.m[3], a * b.m[4], a * b.m[5] );
                                                                  }

        friend vec2 operator*(const mat3x2 &mat, const vec3 &v3 ) { 
                                                                    return vec2( mat.m[0] * v3.v[0] + mat.m[2] * v3.v[1] + mat.m[4] * v3.v[2],
                                                                                 mat.m[1] * v3.v[0] + mat.m[3] * v3.v[1] + mat.m[5] * v3.v[2] );
                                                                  }

        friend vec3 operator*(const vec2 &v2, const mat3x2 &mat)  { 
                                                                    return vec3( v2.v[0] * mat.m[0] + v2.v[1] * mat.m[1],
                                                                                 v2.v[0] * mat.m[2] + v2.v[1] * mat.m[3],
                                                                                 v2.v[0] * mat.m[4] + v2.v[1] * mat.m[5] );
                                                                  }

        friend mat3x2 operator*(const mat2 &a, const mat3x2 &b)   { // 2x2 . 2x3 
                                                                    return mat3x2( a.m[0] * b.m[0] + a.m[2] * b.m[1], a.m[1] * b.m[0] + a.m[3] * b.m[1], 
                                                                                   a.m[0] * b.m[2] + a.m[2] * b.m[3], a.m[1] * b.m[2] + a.m[3] * b.m[3], 
                                                                                   a.m[0] * b.m[4] + a.m[2] * b.m[5], a.m[1] * b.m[4] + a.m[3] * b.m[5] );
                                                                  }
        friend mat2 operator*(const mat3x2 &a, const mat2x3 &b)   { 
                                                                    return mat2( a.m[0] * b.m[0] + a.m[2] * b.m[1] + a.m[4] * b.m[2], a.m[1] * b.m[0] + a.m[3] * b.m[1] + a.m[5] * b.m[2],
                                                                                 a.m[0] * b.m[3] + a.m[2] * b.m[4] + a.m[4] * b.m[5], a.m[1] * b.m[3] + a.m[3] * b.m[4] + a.m[5] * b.m[5] );
                                                                  }

        friend mat3x2 operator*(const mat3x2 &a, const mat3 &b)   {
                                                                    return mat3x2( a.m[0] * b.m[0] + a.m[2] * b.m[1] + a.m[4] * b.m[2],
                                                                                   a.m[1] * b.m[0] + a.m[3] * b.m[1] + a.m[5] * b.m[2],

                                                                                   a.m[0] * b.m[3] + a.m[2] * b.m[4] + a.m[4] * b.m[5],
                                                                                   a.m[1] * b.m[3] + a.m[3] * b.m[4] + a.m[5] * b.m[5],

                                                                                   a.m[0] * b.m[6] + a.m[2] * b.m[7] + a.m[4] * b.m[8],
                                                                                   a.m[1] * b.m[6] + a.m[3] * b.m[7] + a.m[5] * b.m[8] );
                                                                  }
        friend mat3 operator*(const mat2x3 &a, const mat3x2 &b)   { 
                                                                    return mat3( a.m[0] * b.m[0] + a.m[3] * b.m[1],
                                                                                 a.m[1] * b.m[0] + a.m[4] * b.m[1],
                                                                                 a.m[2] * b.m[0] + a.m[5] * b.m[1],

                                                                                 a.m[0] * b.m[2] + a.m[3] * b.m[3],
                                                                                 a.m[1] * b.m[2] + a.m[4] * b.m[3],
                                                                                 a.m[2] * b.m[2] + a.m[5] * b.m[3],

                                                                                 a.m[0] * b.m[4] + a.m[3] * b.m[5],
                                                                                 a.m[1] * b.m[4] + a.m[4] * b.m[5],
                                                                                 a.m[2] * b.m[4] + a.m[5] * b.m[5] );
                                                                  }
        friend mat3x2 outerproduct(const vec3 &c, const vec2 &r)  { 
                                                                    return mat3x2( c.v[0] * r.v[0], c.v[0] * r.v[1],
                                                                                   c.v[1] * r.v[0], c.v[1] * r.v[1],
                                                                                   c.v[2] * r.v[0], c.v[2] * r.v[1] );
                                                                  }

        friend mat3x2 transpose(const mat2x3 &mat)                { 
                                                                    return mat3x2( mat.m[0], mat.m[3],
                                                                                   mat.m[1], mat.m[4],
                                                                                   mat.m[2], mat.m[5] );
                                                                  }
        friend mat2x3 transpose(const mat3x2 &mat)                { 
                                                                    return mat2x3( mat.m[0], mat.m[2], mat.m[4], 
                                                                                   mat.m[1], mat.m[3], mat.m[5] );
                                                                  }
        friend mat3x2 inverse(const mat2x3 &mat)                  { //gives the psuedo inverse 
                                                                        return  mat3x2(inverse(transpose(mat) * mat) * transpose(mat));
                                                                  }

       /* friend mat2x3 inverse(const mat3x2 &mat)                { //will have to change to SVD in order to find pseudo inverse
                                                                        return  mat2x3(inverse(transpose(mat) * mat) * transpose(mat));
                                                                  };*/
        operator float *()                                        { return m; }
};


class mat3x4{ //3 columns 4 rows
    public:
        float m[12];
        mat3x4(float diag = 0)                                    { 
                                                                    m[0] = diag; m[1] = 0;    m[2] = 0;
                                                                    m[3] = 0;    m[4] = diag; m[5] = 0;
                                                                    m[6] = 0;    m[7] = 0;    m[8] = diag;
                                                                    m[9] = 0;    m[10]= 0;    m[11]= 0;
                                                                  }

        mat3x4( float c1r1, float c1r2, float c1r3, float c1r4,
                float c2r1, float c2r2, float c2r3, float c2r4,
                float c3r1, float c3r2, float c3r3, float c3r4)   { 
                                                                    m[0] = c1r1; m[4] = c2r1; m[8] =  c3r1;
                                                                    m[1] = c1r2; m[5] = c2r2; m[9] =  c3r2;
                                                                    m[2] = c1r3; m[6] = c2r3; m[10]=  c3r3;
                                                                    m[3] = c1r4; m[7] = c2r4; m[11]=  c3r4;
                                                                  }

        friend vec3 operator*(const vec4 &v4, const mat3x4 &mat)  {
                                                                    return vec3 ( v4.v[0] * mat.m[0] + v4.v[1] * mat.m[1] + v4.v[2] * mat.m[2] + v4.v[3] * mat.m[3],
                                                                                  v4.v[0] * mat.m[4] + v4.v[1] * mat.m[5] + v4.v[2] * mat.m[6] + v4.v[3] * mat.m[7],
                                                                                  v4.v[0] * mat.m[8] + v4.v[1] * mat.m[9] + v4.v[2] * mat.m[10]+ v4.v[3] * mat.m[11] );
                                                                  }

        friend vec4 operator*(const mat3x4 &mat, const vec3 &v3)  { 
                                                                    return vec4 ( mat.m[0] * v3.v[0] + mat.m[4] * v3.v[1] + mat.m[8] * v3.v[2],
                                                                                  mat.m[1] * v3.v[0] + mat.m[5] * v3.v[1] + mat.m[9] * v3.v[2],
                                                                                  mat.m[2] * v3.v[0] + mat.m[6] * v3.v[1] + mat.m[10]* v3.v[2],
                                                                                  mat.m[3] * v3.v[0] + mat.m[7] * v3.v[1] + mat.m[11]* v3.v[2] );
                                                                  }


        friend mat3x4 operator+(const mat3x4 &a, const mat3x4 &b) { 
                                                                    return mat3x4 ( a.m[0] + b.m[0], a.m[1] + b.m[1],  a.m[2] + b.m[2],
                                                                                    a.m[3] + b.m[3], a.m[4] + b.m[4],  a.m[5] + b.m[5],
                                                                                    a.m[6] + b.m[6], a.m[7] + b.m[7],  a.m[8] + b.m[8],
                                                                                    a.m[9] + b.m[9], a.m[10]+ b.m[10], a.m[11]+ b.m[11] );
                                                                  }

        friend mat3x4 operator-(const mat3x4 &a, const mat3x4 &b) { 
                                                                    return mat3x4 ( a.m[0] - b.m[0], a.m[1] - b.m[1],  a.m[2] - b.m[2],
                                                                                    a.m[3] - b.m[3], a.m[4] - b.m[4],  a.m[5] - b.m[5],
                                                                                    a.m[6] - b.m[6], a.m[7] - b.m[7],  a.m[8] - b.m[8],
                                                                                    a.m[9] - b.m[9], a.m[10]- b.m[10], a.m[11]- b.m[11] );
                                                                  }

        friend mat3x4 operator*(const float a, const mat3x4 &mat) { 
                                                                    return mat3x4 ( a * mat.m[0], a * mat.m[1],  a * mat.m[2],
                                                                                    a * mat.m[3], a * mat.m[4],  a * mat.m[5],
                                                                                    a * mat.m[6], a * mat.m[7],  a * mat.m[8],
                                                                                    a * mat.m[9], a * mat.m[10], a * mat.m[11] );
                                                                  }
        friend mat2x4 operator*(const mat3x4 &a, const mat2x3 &b) { 
                                                                    return mat2x4 ( a.m[0] * b.m[0] + a.m[4] * b.m[1] + a.m[8] * b.m[2],
                                                                                    a.m[1] * b.m[0] + a.m[5] * b.m[1] + a.m[9] * b.m[2],
                                                                                    a.m[2] * b.m[0] + a.m[6] * b.m[1] + a.m[10]* b.m[2],
                                                                                    a.m[3] * b.m[0] + a.m[7] * b.m[1] + a.m[11]* b.m[2],

                                                                                    a.m[0] * b.m[3] + a.m[4] * b.m[4] + a.m[8] * b.m[5],
                                                                                    a.m[1] * b.m[3] + a.m[5] * b.m[4] + a.m[9] * b.m[5],
                                                                                    a.m[2] * b.m[3] + a.m[6] * b.m[4] + a.m[10]* b.m[5],
                                                                                    a.m[3] * b.m[3] + a.m[7] * b.m[4] + a.m[11]* b.m[5] );
                                                                  }
        friend mat3x4 operator*(const mat4 &a, const mat3x4 &b)   { // 4x4 . 4x3 
                                                                     return mat3x4( a.m[0] * b.m[0] + a.m[4] * b.m[1] + a.m[8] * b.m[2] + a.m[12]* b.m[3],
                                                                                    a.m[1] * b.m[0] + a.m[5] * b.m[1] + a.m[9] * b.m[2] + a.m[13]* b.m[3],
                                                                                    a.m[2] * b.m[0] + a.m[6] * b.m[1] + a.m[10]* b.m[2] + a.m[14]* b.m[3],
                                                                                    a.m[3] * b.m[0] + a.m[7] * b.m[1] + a.m[11]* b.m[2] + a.m[15]* b.m[3],

                                                                                    a.m[0] * b.m[4] + a.m[4] * b.m[5] + a.m[8] * b.m[6] + a.m[12] * b.m[7],
                                                                                    a.m[1] * b.m[4] + a.m[5] * b.m[5] + a.m[9] * b.m[6] + a.m[13] * b.m[7],
                                                                                    a.m[2] * b.m[4] + a.m[6] * b.m[5] + a.m[10]* b.m[6] + a.m[14] * b.m[7],
                                                                                    a.m[3] * b.m[4] + a.m[7] * b.m[5] + a.m[11]* b.m[6] + a.m[15] * b.m[7],

                                                                                    a.m[0] * b.m[8] + a.m[4] * b.m[9] + a.m[8] * b.m[10] + a.m[12] * b.m[11],
                                                                                    a.m[1] * b.m[8] + a.m[5] * b.m[9] + a.m[9] * b.m[10] + a.m[13] * b.m[11],
                                                                                    a.m[2] * b.m[8] + a.m[6] * b.m[9] + a.m[10]* b.m[10] + a.m[14] * b.m[11],
                                                                                    a.m[3] * b.m[8] + a.m[7] * b.m[9] + a.m[11]* b.m[10] + a.m[15] * b.m[11] );
                                                                  }

        friend mat3x4 outerproduct(vec3 &c, vec4 &r)              { 
                                                                    return mat3x4 ( c.v[0] * r.v[0], c.v[0] * r.v[1], c.v[0] * r.v[2], c.v[0] * r.v[3], 
                                                                                    c.v[1] * r.v[0], c.v[1] * r.v[1], c.v[1] * r.v[2], c.v[1] * r.v[3], 
                                                                                    c.v[2] * r.v[0], c.v[2] * r.v[1], c.v[2] * r.v[2], c.v[2] * r.v[3] );
                                                                  }

        operator float *()                                        { return m; }
};

class mat4x2{ //4 columns 2 rows
    public:
        float m[8];
        mat4x2(float diag = 0)                                    {   
                                                                      m[0]=diag; m[1]=0;    m[2]=0;    m[3]=0;
                                                                      m[4]=0;    m[5]=diag; m[6]=0;    m[7]=0;
                                                                  }

        mat4x2(float c1r1, float c1r2,
               float c2r1, float c2r2,
               float c3r1, float c3r2,
               float c4r1, float c4r2)                            {   
                                                                      m[0]=c1r1;  m[2]=c2r1;  m[4]=c3r1;  m[6]=c4r1;
                                                                      m[1]=c1r2;  m[3]=c2r2;  m[5]=c3r2;  m[7]=c4r2;
                                                                  }

        friend mat4x2 operator+(const mat4x2 &a, const mat4x2 &b) { 
                                                                    return mat4x2( a.m[0] + b.m[0],  a.m[1]+ b.m[1], a.m[2] + b.m[2], a.m[3]+ b.m[3],
                                                                                   a.m[4] + b.m[4],  a.m[5]+ b.m[5], a.m[6] + b.m[6], a.m[7]+ b.m[7]);
                                                                  }

        friend mat4x2 operator-(const mat4x2 &a, const mat4x2 &b) { 
                                                                    return mat4x2( a.m[0] - b.m[0],  a.m[1]- b.m[1], a.m[2] - b.m[2], a.m[3]- b.m[3],
                                                                                   a.m[4] - b.m[4],  a.m[5]- b.m[5], a.m[6] - b.m[6], a.m[7]- b.m[7]);
                                                                  }

        friend mat4x2 operator*(const float a, const mat4x2 &b)   { 
                                                                    return mat4x2( a * b.m[0], a * b.m[1], a * b.m[2], a * b.m[3],
                                                                                   a * b.m[4], a * b.m[5], a * b.m[6], a * b.m[7] );
                                                                  }

        friend vec2 operator*(const mat4x2 &mat, const vec4 &v4)  { 
                                                                    return vec2( mat.m[0] * v4.v[0] + mat.m[2] * v4.v[1] + mat.m[4] * v4.v[2] + mat.m[6] * v4.v[3],
                                                                                 mat.m[1] * v4.v[0] + mat.m[3] * v4.v[1] + mat.m[5] * v4.v[2] + mat.m[7] * v4.v[3]);
                                                                  }

        friend vec4 operator*(const vec2 &v2, const mat4x2 &mat)  { 
                                                                    return vec4( v2.v[0] * mat.m[0] + v2.v[1] * mat.m[1],
                                                                                 v2.v[0] * mat.m[2] + v2.v[1] * mat.m[3],
                                                                                 v2.v[0] * mat.m[4] + v2.v[1] * mat.m[5],
                                                                                 v2.v[0] * mat.m[6] + v2.v[1] * mat.m[7]
                                                                               );
                                                                  }
        friend mat4x2 operator*(const mat2 &a, const mat4x2 &b)   { //2x2 . 2x4 
                                                                    return mat4x2( a.m[0] * b.m[0] + a.m[2] * b.m[1], a.m[1] * b.m[0] + a.m[3] * b.m[1], 
                                                                                   a.m[0] * b.m[2] + a.m[2] * b.m[3], a.m[1] * b.m[2] + a.m[3] * b.m[3],
                                                                                   a.m[0] * b.m[4] + a.m[2] * b.m[5], a.m[1] * b.m[4] + a.m[3] * b.m[5], 
                                                                                   a.m[0] * b.m[6] + a.m[2] * b.m[7], a.m[1] * b.m[6] + a.m[3] * b.m[7] );
                                                                  }
        friend mat2 operator*(const mat4x2 &a, const mat2x4 &b)   { 
                                                                    return mat2( a.m[0] * b.m[0] + a.m[2] * b.m[1] + a.m[4] * b.m[2] + a.m[6] * b.m[3],
                                                                                 a.m[1] * b.m[0] + a.m[3] * b.m[1] + a.m[5] * b.m[2] + a.m[7] * b.m[3],

                                                                                 a.m[0] * b.m[4] + a.m[2] * b.m[5] + a.m[4] * b.m[6] + a.m[6] * b.m[7],
                                                                                 a.m[1] * b.m[4] + a.m[3] * b.m[5] + a.m[5] * b.m[6] + a.m[7] * b.m[7] );
                                                                  }

        friend mat3x2 operator*(const mat4x2 &a, const mat3x4 &b) { 
                                                                    return mat3x2( a.m[0] * b.m[0] + a.m[2] * b.m[1] + a.m[4] * b.m[2] + a.m[6] * b.m[3],
                                                                                   a.m[1] * b.m[0] + a.m[3] * b.m[1] + a.m[5] * b.m[2] + a.m[7] * b.m[3],

                                                                                   a.m[0] * b.m[4] + a.m[2] * b.m[5] + a.m[4] * b.m[6] + a.m[6] * b.m[7],
                                                                                   a.m[1] * b.m[4] + a.m[3] * b.m[5] + a.m[5] * b.m[6] + a.m[7] * b.m[7],

                                                                                   a.m[0] * b.m[8] + a.m[2] * b.m[9] + a.m[4] * b.m[10]+ a.m[6] * b.m[11],
                                                                                   a.m[1] * b.m[8] + a.m[3] * b.m[9] + a.m[5] * b.m[10]+ a.m[7] * b.m[11] );
                                                                  }

        friend mat4x2 operator*(const mat4x2 &a, const mat4 &b)   { 
                                                                    return mat4x2( a.m[0] * b.m[0] + a.m[2] * b.m[1] + a.m[4] * b.m[2] + a.m[6] * b.m[3],
                                                                                   a.m[1] * b.m[0] + a.m[3] * b.m[1] + a.m[5] * b.m[2] + a.m[7] * b.m[3],

                                                                                   a.m[0] * b.m[4] + a.m[2] * b.m[5] + a.m[4] * b.m[6] + a.m[6] * b.m[7],
                                                                                   a.m[1] * b.m[4] + a.m[3] * b.m[5] + a.m[5] * b.m[6] + a.m[7] * b.m[7],

                                                                                   a.m[0] * b.m[8] + a.m[2] * b.m[9] + a.m[4] * b.m[10]+ a.m[6] * b.m[11],
                                                                                   a.m[1] * b.m[8] + a.m[3] * b.m[9] + a.m[5] * b.m[10]+ a.m[7] * b.m[11],

                                                                                   a.m[0] * b.m[12]+ a.m[2] * b.m[13]+ a.m[4] * b.m[14]+ a.m[6] * b.m[15],
                                                                                   a.m[1] * b.m[12]+ a.m[3] * b.m[13]+ a.m[5] * b.m[14]+ a.m[7] * b.m[15] );
                                                                  }

        friend mat4 operator*(const mat2x4 &a, const mat4x2 &b)   { 
                                                                    return mat4( a.m[0] * b.m[0] + a.m[4] * b.m[1],
                                                                                 a.m[1] * b.m[0] + a.m[5] * b.m[1],
                                                                                 a.m[2] * b.m[0] + a.m[6] * b.m[1],
                                                                                 a.m[3] * b.m[0] + a.m[7] * b.m[1],

                                                                                 a.m[0] * b.m[2] + a.m[4] * b.m[3], 
                                                                                 a.m[1] * b.m[2] + a.m[5] * b.m[3],
                                                                                 a.m[2] * b.m[2] + a.m[6] * b.m[3], 
                                                                                 a.m[3] * b.m[2] + a.m[7] * b.m[3],

                                                                                 a.m[0] * b.m[4] + a.m[4] * b.m[5], 
                                                                                 a.m[1] * b.m[4] + a.m[5] * b.m[5],
                                                                                 a.m[2] * b.m[4] + a.m[6] * b.m[5], 
                                                                                 a.m[3] * b.m[4] + a.m[7] * b.m[5],

                                                                                 a.m[0] * b.m[6] + a.m[4] * b.m[7], 
                                                                                 a.m[1] * b.m[6] + a.m[5] * b.m[7],
                                                                                 a.m[2] * b.m[6] + a.m[6] * b.m[7], 
                                                                                 a.m[3] * b.m[6] + a.m[7] * b.m[7] );
                                                                  }

        friend mat4x2 outerproduct(const vec4 &c,const vec2 &r)   { 
                                                                    return mat4x2( c.v[0] * r.v[0], c.v[0] * r.v[1], 
                                                                                   c.v[1] * r.v[0], c.v[1] * r.v[1],
                                                                                   c.v[2] * r.v[0], c.v[2] * r.v[1], 
                                                                                   c.v[3] * r.v[0], c.v[3] * r.v[1]);
                                                                  }

        friend mat2x4 transpose(const mat4x2 &mat)                {
                                                                    return mat2x4( mat.m[0], mat.m[2],
                                                                                   mat.m[4], mat.m[6],
                                                                                   mat.m[1], mat.m[3],
                                                                                   mat.m[5], mat.m[7] );
                                                                  }

        friend mat4x2 transpose(const mat2x4 &mat)                { 
                                                                    return mat4x2( mat.m[0], mat.m[4], mat.m[1], mat.m[5],
                                                                                   mat.m[2], mat.m[6], mat.m[3], mat.m[7] );
                                                                  }

        friend mat4x2 inverse(const mat2x4 &mat)                  {
                                                                      return  mat4x2(inverse(transpose(mat) * mat) * transpose(mat));
                                                                  }

        /*friend mat2x4 inverse(const mat4x2 &mat)                { //will have to use SVD to find pseudo inverse
                                                                      return  mat2x4(inverse(transpose(mat) * mat) * transpose(mat));
                                                                  };*/
        operator float *()                                        { return m; }
};



class mat4x3{ //4 columns 3 rows -- 3x4
    public:
        float m[12];
        mat4x3(float diag = 0)                                    { 
                                                                    m[0] = diag; m[1] = 0;    m[2] = 0;    m[3] = 0;
                                                                    m[4] = 0;    m[5] = diag; m[6] = 0;    m[7] = 0;
                                                                    m[8] = 0;    m[9] = 0;    m[10]= diag; m[11]= 0;
                                                                  }

        mat4x3( float c1r1, float c1r2, float c1r3,
                float c2r1, float c2r2, float c2r3,
                float c3r1, float c3r2, float c3r3, 
                float c4r1, float c4r2, float c4r3 )              { 
                                                                     m[0] = c1r1; m[3] = c2r1; m[6] = c3r1; m[9] = c4r1;
                                                                     m[1] = c1r2; m[4] = c2r2; m[7] = c3r2; m[10]= c4r2;
                                                                     m[2] = c1r3; m[5] = c2r3; m[8] = c3r3; m[11]= c4r3;
                                                                  }

        friend mat4x3 operator+(const mat4x3 &a, const mat4x3 &b) { 
                                                                    return mat4x3 ( a.m[0] + b.m[0], a.m[1] + b.m[1], a.m[2] + b.m[2] , a.m[3] + b.m[3],
                                                                                    a.m[4] + b.m[4], a.m[5] + b.m[5], a.m[6] + b.m[6] , a.m[7] + b.m[7],
                                                                                    a.m[8] + b.m[8], a.m[9] + b.m[9], a.m[10]+ b.m[10], a.m[11]+ b.m[11] );
                                                                  }

        friend mat4x3 operator-(const mat4x3 &a, const mat4x3 &b) { 
                                                                    return mat4x3 ( a.m[0] + b.m[0], a.m[1] + b.m[1], a.m[2] + b.m[2] , a.m[3] + b.m[3],
                                                                                    a.m[4] + b.m[4], a.m[5] + b.m[5], a.m[6] + b.m[6] , a.m[7] + b.m[7],
                                                                                    a.m[8] + b.m[8], a.m[9] + b.m[9], a.m[10]+ b.m[10], a.m[11]+ b.m[11] );
                                                                  }

        friend mat4x3 operator*(const float a, const mat4x3 &mat) { 
                                                                    return mat4x3 ( a * mat.m[0], a * mat.m[1], a * mat.m[2],  a * mat.m[3],
                                                                                    a * mat.m[4], a * mat.m[5], a * mat.m[6],  a * mat.m[7],
                                                                                    a * mat.m[8], a * mat.m[9], a * mat.m[10], a * mat.m[11] );
                                                                  }
        friend vec3 operator*(const mat4x3 &mat, const vec4 &v4)  { 
                                                                    return vec3 ( mat.m[0] * v4.v[0] + mat.m[3] * v4.v[1] + mat.m[6] * v4.v[2] + mat.m[9] * v4.v[3],
                                                                                  mat.m[1] * v4.v[0] + mat.m[4] * v4.v[1] + mat.m[7] * v4.v[2] + mat.m[10]* v4.v[3],
                                                                                  mat.m[2] * v4.v[0] + mat.m[5] * v4.v[1] + mat.m[8] * v4.v[2] + mat.m[11]* v4.v[3] );
                                                                  }

        friend vec4 operator*(const vec3 &v3, const mat4x3 &mat)  { 
                                                                    return vec4 ( v3.v[0] * mat.m[0] + v3.v[1] * mat.m[1] + v3.v[2] * mat.m[2],
                                                                                  v3.v[0] * mat.m[3] + v3.v[1] * mat.m[4] + v3.v[2] * mat.m[5],
                                                                                  v3.v[0] * mat.m[6] + v3.v[1] * mat.m[7] + v3.v[2] * mat.m[8],
                                                                                  v3.v[0] * mat.m[9] + v3.v[1] * mat.m[10]+ v3.v[2] * mat.m[11] );
                                                                  }

        friend mat3 operator*(const mat4x3 &a, const mat3x4 &b)   { //3x4.4x3 
                                                                     return mat3 ( a.m[0] * b.m[0] + a.m[3] * b.m[1] + a.m[6] * b.m[2] + a.m[9] * b.m[3],
                                                                                   a.m[1] * b.m[0] + a.m[4] * b.m[1] + a.m[7] * b.m[2] + a.m[10]* b.m[3],
                                                                                   a.m[2] * b.m[0] + a.m[5] * b.m[1] + a.m[8] * b.m[2] + a.m[11]* b.m[3],

                                                                                   a.m[0] * b.m[4] + a.m[3] * b.m[5] + a.m[6] * b.m[6] + a.m[9] * b.m[7],
                                                                                   a.m[1] * b.m[4] + a.m[4] * b.m[5] + a.m[7] * b.m[6] + a.m[10]* b.m[7],
                                                                                   a.m[2] * b.m[4] + a.m[5] * b.m[5] + a.m[8] * b.m[6] + a.m[11]* b.m[7],

                                                                                   a.m[0] * b.m[8] + a.m[3] * b.m[9] + a.m[6] * b.m[10]+ a.m[9] * b.m[11],
                                                                                   a.m[1] * b.m[8] + a.m[4] * b.m[9] + a.m[7] * b.m[10]+ a.m[10]* b.m[11],
                                                                                   a.m[2] * b.m[8] + a.m[5] * b.m[9] + a.m[8] * b.m[10]+ a.m[11]* b.m[11] );
                                                                  }

        friend mat4 operator*(const mat3x4 &a, const mat4x3 &b)   { 
                                                                     return mat4( a.m[0] * b.m[0] + a.m[4] * b.m[1] + a.m[8] * b.m[2],
                                                                                  a.m[1] * b.m[0] + a.m[5] * b.m[1] + a.m[9] * b.m[2],
                                                                                  a.m[2] * b.m[0] + a.m[6] * b.m[1] + a.m[10]* b.m[2],
                                                                                  a.m[3] * b.m[0] + a.m[7] * b.m[1] + a.m[11]* b.m[2],

                                                                                  a.m[0] * b.m[3] + a.m[4] * b.m[4] + a.m[8] * b.m[5],
                                                                                  a.m[1] * b.m[3] + a.m[5] * b.m[4] + a.m[9] * b.m[5],
                                                                                  a.m[2] * b.m[3] + a.m[6] * b.m[4] + a.m[10]* b.m[5],
                                                                                  a.m[3] * b.m[3] + a.m[7] * b.m[4] + a.m[11]* b.m[5],

                                                                                  a.m[0] * b.m[6] + a.m[4] * b.m[7] + a.m[8] * b.m[8],
                                                                                  a.m[1] * b.m[6] + a.m[5] * b.m[7] + a.m[9] * b.m[8],
                                                                                  a.m[2] * b.m[6] + a.m[6] * b.m[7] + a.m[10]* b.m[8],
                                                                                  a.m[3] * b.m[6] + a.m[7] * b.m[7] + a.m[11]* b.m[8],

                                                                                  a.m[0] * b.m[9] + a.m[4] * b.m[10]+ a.m[8]  * b.m[11],
                                                                                  a.m[1] * b.m[9] + a.m[5] * b.m[10]+ a.m[9]  * b.m[11],
                                                                                  a.m[2] * b.m[9] + a.m[6] * b.m[10]+ a.m[10] * b.m[11],
                                                                                  a.m[3] * b.m[9] + a.m[7] * b.m[10]+ a.m[11] * b.m[11] );
                                                                  }

        friend mat4x3 operator*(const mat3 &a, const mat4x3 &b)   { //3x3 . 3x4 
                                                                    return mat4x3( a.m[0] * b.m[0] + a.m[3] * b.m[1] + a.m[6] * b.m[2],
                                                                                   a.m[1] * b.m[0] + a.m[4] * b.m[1] + a.m[7] * b.m[2],
                                                                                   a.m[2] * b.m[0] + a.m[5] * b.m[1] + a.m[8] * b.m[2],

                                                                                   a.m[0] * b.m[3] + a.m[3] * b.m[4] + a.m[6] * b.m[5],
                                                                                   a.m[1] * b.m[3] + a.m[4] * b.m[4] + a.m[7] * b.m[5],
                                                                                   a.m[2] * b.m[3] + a.m[5] * b.m[4] + a.m[8] * b.m[5],

                                                                                   a.m[0] * b.m[6] + a.m[3] * b.m[7] + a.m[6] * b.m[8],
                                                                                   a.m[1] * b.m[6] + a.m[4] * b.m[7] + a.m[7] * b.m[8],
                                                                                   a.m[2] * b.m[6] + a.m[5] * b.m[7] + a.m[8] * b.m[8],

                                                                                   a.m[0] * b.m[9] + a.m[3] * b.m[10]+ a.m[6] * b.m[11],
                                                                                   a.m[1] * b.m[9] + a.m[4] * b.m[10]+ a.m[7] * b.m[11],
                                                                                   a.m[2] * b.m[9] + a.m[5] * b.m[10]+ a.m[8] * b.m[11] );
                                                                  }

        friend mat2x3 operator*(const mat4x3 &a, const mat2x4 &b) { //3x4.4x2 
                                                                    return mat2x3 ( a.m[0] * b.m[0] + a.m[3] * b.m[1] + a.m[6] * b.m[2] + a.m[9] * b.m[3],
                                                                                    a.m[1] * b.m[0] + a.m[4] * b.m[1] + a.m[7] * b.m[2] + a.m[10]* b.m[3],
                                                                                    a.m[2] * b.m[0] + a.m[5] * b.m[1] + a.m[8] * b.m[2] + a.m[11]* b.m[3],

                                                                                    a.m[0] * b.m[4] + a.m[3] * b.m[5] + a.m[6] * b.m[6] + a.m[9] * b.m[7],
                                                                                    a.m[1] * b.m[4] + a.m[4] * b.m[5] + a.m[7] * b.m[6] + a.m[10]* b.m[7],
                                                                                    a.m[2] * b.m[4] + a.m[5] * b.m[5] + a.m[8] * b.m[6] + a.m[11]* b.m[7] );
                                                                  }

        friend mat4x3 operator*(const mat4x3 &a, const mat4 &b)   { //3x4 . 4x4 
                                                                    return mat4x3 ( a.m[0] * b.m[0] + a.m[1] * b.m[4] + a.m[2] * b.m[8] + a.m[3] * b.m[12],
                                                                                    a.m[0] * b.m[1] + a.m[1] * b.m[5] + a.m[2] * b.m[9] + a.m[3] * b.m[13],
                                                                                    a.m[0] * b.m[2] + a.m[1] * b.m[6] + a.m[2] * b.m[10]+ a.m[3] * b.m[14],
                                                                                    a.m[0] * b.m[3] + a.m[1] * b.m[7] + a.m[2] * b.m[11]+ a.m[3] * b.m[15],

                                                                                    a.m[4] * b.m[0] + a.m[5] * b.m[4] + a.m[6] * b.m[8] + a.m[7] * b.m[12],
                                                                                    a.m[4] * b.m[1] + a.m[5] * b.m[5] + a.m[6] * b.m[9] + a.m[7] * b.m[13],
                                                                                    a.m[4] * b.m[2] + a.m[5] * b.m[6] + a.m[6] * b.m[10]+ a.m[7] * b.m[14],
                                                                                    a.m[4] * b.m[3] + a.m[5] * b.m[7] + a.m[6] * b.m[11]+ a.m[7] * b.m[15],

                                                                                    a.m[8] * b.m[0] + a.m[9] * b.m[4] + a.m[10]* b.m[8] + a.m[11]* b.m[12],
                                                                                    a.m[8] * b.m[1] + a.m[9] * b.m[5] + a.m[10]* b.m[9] + a.m[11]* b.m[13],
                                                                                    a.m[8] * b.m[2] + a.m[9] * b.m[6] + a.m[10]* b.m[10]+ a.m[11]* b.m[14],
                                                                                    a.m[8] * b.m[3] + a.m[9] * b.m[7] + a.m[10]* b.m[11]+ a.m[11]* b.m[15] );
                                                                  }

        friend mat4x2 operator*(const mat3x2 &a, const mat4x3 &b) {  
                                                                    return mat4x2( a.m[0] * b.m[0] + a.m[2] * b.m[1] + a.m[4] * b.m[2],
                                                                                   a.m[1] * b.m[0] + a.m[3] * b.m[1] + a.m[5] * b.m[2],

                                                                                   a.m[0] * b.m[3] + a.m[2] * b.m[4] + a.m[4] * b.m[5],
                                                                                   a.m[1] * b.m[3] + a.m[3] * b.m[4] + a.m[5] * b.m[5],

                                                                                   a.m[0] * b.m[6] + a.m[2] * b.m[7] + a.m[4] * b.m[8],
                                                                                   a.m[1] * b.m[6] + a.m[3] * b.m[7] + a.m[5] * b.m[8],

                                                                                   a.m[0] * b.m[9] + a.m[2] * b.m[10]+ a.m[4] * b.m[11],
                                                                                   a.m[1] * b.m[9] + a.m[3] * b.m[10]+ a.m[5] * b.m[11] );
                                                                  }
        friend mat4x3 operator*(const mat2x3 &a, const mat4x2 &b) { //not tested
                                                                    return mat4x3( a.m[0] * b.m[0] + a.m[3] * b.m[1],
                                                                                   a.m[1] * b.m[0] + a.m[4] * b.m[1],
                                                                                   a.m[2] * b.m[0] + a.m[5] * b.m[1],

                                                                                   a.m[0] * b.m[2] + a.m[3] * b.m[3],
                                                                                   a.m[1] * b.m[2] + a.m[4] * b.m[3],
                                                                                   a.m[2] * b.m[2] + a.m[5] * b.m[3],

                                                                                   a.m[0] * b.m[4] + a.m[3] * b.m[5],
                                                                                   a.m[1] * b.m[4] + a.m[4] * b.m[5],
                                                                                   a.m[2] * b.m[4] + a.m[5] * b.m[5],

                                                                                   a.m[0] * b.m[6] + a.m[3] * b.m[7],
                                                                                   a.m[1] * b.m[6] + a.m[4] * b.m[7],
                                                                                   a.m[2] * b.m[6] + a.m[5] * b.m[7] );
                                                                  }
        friend mat4x3 outerproduct(vec4 &c, vec3 &r)              { 
                                                                    return mat4x3 ( c.v[0] * r.v[0], c.v[0] * r.v[1], c.v[0] * r.v[2],
                                                                                    c.v[1] * r.v[0], c.v[1] * r.v[1], c.v[1] * r.v[2],
                                                                                    c.v[2] * r.v[0], c.v[2] * r.v[1], c.v[2] * r.v[2],
                                                                                    c.v[3] * r.v[0], c.v[3] * r.v[1], c.v[3] * r.v[2] );
                                                                  }

        friend mat3x4 transpose(const mat4x3 &mat)                { 
                                                                    return mat3x4 ( mat.m[0], mat.m[3], mat.m[6], mat.m[9],
                                                                                    mat.m[1], mat.m[4], mat.m[7], mat.m[10],
                                                                                    mat.m[2], mat.m[5], mat.m[8], mat.m[11] );
                                                                  }

        friend mat4x3 transpose(const mat3x4 &mat)                { 
                                                                    return mat4x3 ( mat.m[0], mat.m[4], mat.m[8],
                                                                                    mat.m[1], mat.m[5], mat.m[9],
                                                                                    mat.m[2], mat.m[6], mat.m[10],
                                                                                    mat.m[3], mat.m[7], mat.m[11] );
                                                                  }
        friend mat4x3 inverse( const mat3x4 &mat)                 { //psuedo inverse
                                                                    return mat4x3(inverse(transpose(mat) * mat) * transpose(mat));
                                                                  }
       /*friend mat3x4 inverse( const mat4x3 &mat)                 { //will have to use SVD in order to find pseudo inverse
                                                                    return mat3x4(inverse(transpose(mat) * mat) * transpose(mat));
                                                                  };*/

        operator float *()                                      { return m; }
};


#endif // __VEC_MATH_H__
