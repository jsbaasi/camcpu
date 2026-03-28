#ifndef MATH_H
#define MATH_H

#include <stdbool.h>

typedef struct {
    float x,y,z,w;
} Vec4;

// optimised for getting each row (store row together)
typedef struct {
    float x0,x1,x2,x3;
    float y0,y1,y2,y3;
    float z0,z1,z2,z3;
    float w0,w1,w2,w3;
} Mat4;

typedef struct {
    float x,y,z;
} Vec3;

// function naming represents the multiplication order
void mat4_vec4_mult(Mat4* m, Vec4* v, Vec4* res);
void vec4_dehomo(Vec4* v, Vec4* res);
void vec4_print(Vec4* v);
bool vec4_equals(Vec4* a, Vec4* b);
bool vec3_equals(Vec3* a, Vec3* b);
void vec4_add(Vec4* a, Vec4* b, Vec4* res);
void vec4_scalar_add(Vec4* a, Vec4* b, float scalar, Vec4* res);
void vec4_scalar_subtract(Vec4* a, Vec4* b, float scalar, Vec4* res);
void square_print(Vec4* square);

#endif
