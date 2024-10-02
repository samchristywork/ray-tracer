#ifndef VEC_H
#define VEC_H

typedef struct {
  float x, y, z;
} Vec3;

Vec3 vec3(float x, float y, float z);

Vec3 vec_add(Vec3 a, Vec3 b);

Vec3 vec_sub(Vec3 a, Vec3 b);

Vec3 vec_scale(Vec3 a, float s);

float vec_dot(Vec3 a, Vec3 b);

Vec3 vec_norm(Vec3 a);

Vec3 vec_cross(Vec3 a, Vec3 b);

#endif
