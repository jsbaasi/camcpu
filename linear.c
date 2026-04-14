#include "linear.h"
#include <stdio.h>
#include <math.h>

void mat4_vec4_mult(Mat4* m, Vec4* v, Vec4* res) {
    res->x = m->x0*v->x + m->x1*v->y + m->x2*v->z + m->x3*v->w;
    res->y = m->y0*v->x + m->y1*v->y + m->y2*v->z + m->y3*v->w;
    res->z = m->z0*v->x + m->z1*v->y + m->z2*v->z + m->z3*v->w;
    res->w = m->w0*v->x + m->w1*v->y + m->w2*v->z + m->w3*v->w;
}

void vec4_dehomo(Vec4* v, Vec4* res) {
    res->x = v->x/v->w;
    res->y = v->y/v->w;
    res->z = v->z/v->w;
    res->w = 1;
}

bool vec4_equals(Vec4* a, Vec4* b) {
    return (
        (a->x == b->x) &&
        (a->y == b->y) &&
        (a->z == b->z) &&
        (a->w == b->w)
    );
}

bool vec3_equals(Vec3* a, Vec3* b) {
    return (
        (a->x == b->x) &&
        (a->y == b->y) &&
        (a->z == b->z)
    );
}

void vec4_print(Vec4* v) {
    printf("Vec4(x=%f,y=%f,z=%f,w=%f)\n", v->x, v->y, v->z, v->w);
}

void vec4_add(Vec4* a, Vec4* b, Vec4* res) {
    res->x = a->x + b->x;
    res->y = a->y + b->y;
    res->z = a->z + b->z;
}

void vec4_scalar_add(Vec4* a, Vec4* b, float scalar, Vec4* res) {
    res->x = a->x + scalar*b->x;
    res->y = a->y + scalar*b->y;
    res->z = a->z + scalar*b->z;
}

void vec4_scalar_subtract(Vec4* a, Vec4* b, float scalar, Vec4* res) {
    res->x = a->x - scalar*b->x;
    res->y = a->y - scalar*b->y;
    res->z = a->z - scalar*b->z;
}

void square_print(Vec4* square) {
    printf("Square(\n");
    for (int i=0; i<4; ++i) {
        printf("Point %d x=%f y=%f z=%f w=%f\n", i+1, square[i].x, square[i].y, square[i].z, square[i].w);
    }
    printf(")\n");
}

double degrees_to_radians(double deg) {
    return deg * M_PI / 180.0;
}

void vec4_cross_product(Vec4* a, Vec4* b, Vec4* res) {
    
}

void set_camera_basis(Vec4* cameraForwardBasis, Vec4* cameraUpBasis, Vec4* cameraLeftBasis, double yaw, double pitch) {
    cameraForwardBasis->x = cos(degrees_to_radians(yaw)) * cos(degrees_to_radians(pitch));
    cameraForwardBasis->y = sin(degrees_to_radians(pitch));
    cameraForwardBasis->z = sin(degrees_to_radians(yaw)) * cos(degrees_to_radians(pitch));
    vec4_normalise(cameraUpBasis);

    cameraUpBasis->x = 0.0;
    cameraUpBasis->y = 1.0;
    cameraUpBasis->z = 0.0;

    vec4_cross_product(cameraForwardBasis, cameraUpBasis, cameraLeftBasis);
    vec4_cross_product(cameraLeftBasis, cameraForwardBasis, cameraUpBasis);
}

void vec4_normalise(Vec4* v) {
    double magnitude = sqrt(pow(v->x, 2.0) + pow(v->y, 2.0) + pow(v->z, 2.0));
    v->x /= magnitude;
    v->y /= magnitude;
    v->z /= magnitude;
}
