#ifndef SHAPE_H
#define SHAPE_H

#include "vec.h"

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

int sphere_intersect(Sphere sphere, Vec3 origin, Vec3 direction, float *t);

int plane_intersect(Plane plane, Vec3 origin, Vec3 direction, float *t);

int triangle_intersect(Triangle triangle, Vec3 origin, Vec3 direction, float *t,
                       Vec3 *hit_normal);

#endif
