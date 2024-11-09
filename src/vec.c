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

Vec3 vec_rot(Vec3 v, Vec3 axis, float angle) {
  axis = vec_norm(axis);
  float cos_theta = cosf(angle);
  float sin_theta = sinf(angle);
  Vec3 term1 = vec_scale(v, cos_theta);
  Vec3 term2 = vec_scale(vec_cross(axis, v), sin_theta);
  Vec3 term3 = vec_scale(axis, vec_dot(axis, v) * (1.0f - cos_theta));

  return vec_add(vec_add(term1, term2), term3);
}
