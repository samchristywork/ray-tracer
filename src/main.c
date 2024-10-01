#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>

#define IMAGE_WIDTH 1600
#define IMAGE_HEIGHT 1600
#define MAX_RECURSION_DEPTH 10

typedef struct {
    float x, y, z;
} Vec3;

typedef struct {
    Vec3 center;
    float radius;
    Vec3 color;
    float reflection;
} Sphere;

typedef struct {
    Vec3 normal;
    float d;
    Vec3 color;
    float reflection;
} Plane;

typedef struct {
    Vec3 v0, v1, v2;
    Vec3 color;
    float reflection;
} Triangle;

Vec3 vec3(float x, float y, float z) {
    return (Vec3){x, y, z};
}

Vec3 vec_add(Vec3 a, Vec3 b) {
    return vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

Vec3 vec_sub(Vec3 a, Vec3 b) {
    return vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

Vec3 vec_scale(Vec3 a, float s) {
    return vec3(a.x * s, a.y * s, a.z * s);
}

float vec_dot(Vec3 a, Vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3 vec_norm(Vec3 a) {
    float mag = sqrtf(vec_dot(a, a));
    return vec_scale(a, 1.0f / mag);
}

Vec3 vec_cross(Vec3 a, Vec3 b) {
    return vec3(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

float clamp(float x, float lower, float upper) {
    if (x < lower) return lower;
    if (x > upper) return upper;
    return x;
}

int main() {
    unsigned char *image = malloc(IMAGE_WIDTH * IMAGE_HEIGHT * 3);
    if (image == NULL) {
        fprintf(stderr, "Failed to allocate memory for the image\n");
        return 1;
    }

    free(image);
}
