#include "math.h"
#include <assert.h>
#include <stdbool.h>

int main() {
    Mat4 m = {
        1,2,3,4,
        5,6,7,8,
        9,10,11,12,
        13,14,15,16,
    };
    Vec4 v = {
        3,
        4,
        5,
        6,
    };
    Vec4 res = {0,0,0,0};
    mat4_vec4_mult(&m, &v, &res);
    Vec4 expRes = {50,122,194,266};
    assert(vec4_equals(&res, &expRes)==true);

    Vec4 res1= {0,0,0,0};
    Vec4 hv = {6,8,10,2};
    vec4_dehomo(&hv, &res1);
    Vec4 expRes1 = {3,4,5,1};
    assert(vec4_equals(&res1, &expRes1));
}
