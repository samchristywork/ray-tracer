#include <math.h>

#include "vec.h"

Vec3 vec3(float x, float y, float z) { return (Vec3){x, y, z}; }

Vec3 vec_add(Vec3 a, Vec3 b) { return vec3(a.x + b.x, a.y + b.y, a.z + b.z); }

Vec3 vec_sub(Vec3 a, Vec3 b) { return vec3(a.x - b.x, a.y - b.y, a.z - b.z); }

Vec3 vec_scale(Vec3 a, float s) { return vec3(a.x * s, a.y * s, a.z * s); }

float vec_dot(Vec3 a, Vec3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

Vec3 vec_norm(Vec3 a) {
  float mag = sqrtf(vec_dot(a, a));
  return vec_scale(a, 1.0f / mag);
}

Vec3 vec_cross(Vec3 a, Vec3 b) {
  return vec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z,
              a.x * b.y - a.y * b.x);
}
