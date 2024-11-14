#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "shape.h"

int sphere_intersect(Sphere sphere, Vec3 origin, Vec3 direction, float *t) {
  Vec3 oc = vec_sub(origin, sphere.center);
  float a = vec_dot(direction, direction);
  float b = 2.0f * vec_dot(oc, direction);
  float c = vec_dot(oc, oc) - sphere.radius * sphere.radius;
  float discriminant = b * b - 4 * a * c;

  if (discriminant > 0) {
    float sqrtd = sqrtf(discriminant);
    float t0 = (-b - sqrtd) / (2.0f * a);
    float t1 = (-b + sqrtd) / (2.0f * a);
    *t = fminf(t0, t1);

    return (*t >= 0);
  }

  return 0;
}

int plane_intersect(Plane plane, Vec3 origin, Vec3 direction, float *t) {
  float denom = vec_dot(plane.normal, direction);
  if (fabs(denom) > 1e-6) { // Ensure this is not zero
    Vec3 p0l0 = vec_scale(plane.normal, plane.d);
    *t = vec_dot(p0l0, plane.normal) / denom;
    return (*t >= 0);
  }
  return 0;
}

float triangle_area(Vec3 a, Vec3 b, Vec3 c) {
  return vec_norm(vec_cross(vec_sub(b, a), vec_sub(c, a))).x / 2;
}

int triangle_intersect(Triangle triangle, Vec3 origin, Vec3 direction, float *t,
                       Vec3 *hit_normal) {
  Vec3 edge1 = vec_sub(triangle.v1, triangle.v0);
  Vec3 edge2 = vec_sub(triangle.v2, triangle.v0);
  Vec3 h = vec_cross(direction, edge2);
  float a = vec_dot(edge1, h);
  if (a > -1e-6 && a < 1e-6)
    return 0; // This ray is parallel to this triangle.
  float f = 1.0f / a;
  Vec3 s = vec_sub(origin, triangle.v0);
  float u = f * vec_dot(s, h);
  if (u < 0.0 || u > 1.0)
    return 0;
  Vec3 q = vec_cross(s, edge1);
  float v = f * vec_dot(direction, q);
  if (v < 0.0 || u + v > 1.0)
    return 0;
  *t = f * vec_dot(edge2, q);
  if (*t > 1e-6) { // ray intersection
    *hit_normal = vec_norm(vec_cross(edge1, edge2));
    return 1;
  }
  return 0;
}

Triangle *generateBox(int *n) {
  Vec3 colors[6] = {
      vec3(1.0, 0.0, 0.0), // Red
      vec3(0.0, 1.0, 0.0), // Green
      vec3(0.0, 0.0, 1.0), // Blue
      vec3(1.0, 1.0, 0.0), // Yellow
      vec3(0.0, 1.0, 1.0), // Cyan
      vec3(1.0, 0.0, 1.0)  // Magenta
  };

  Vec3 corners[8] = {
      vec3(-0.5, -0.5, -0.5),
      vec3(0.5, -0.5, -0.5),
      vec3(0.5, 0.5, -0.5),
      vec3(-0.5, 0.5, -0.5),
      vec3(-0.5, -0.5, 0.5),
      vec3(0.5, -0.5, 0.5),
      vec3(0.5, 0.5, 0.5),
      vec3(-0.5, 0.5, 0.5)
  };

  for (int i = 0; i < 8; i++) {
    corners[i] = vec_scale(corners[i], 0.25);
    corners[i] = vec_rot(corners[i], vec3(1, 0, 0), 3.141 / 4);
    corners[i] = vec_rot(corners[i], vec3(0, 1, 0), 3.141 / 4);
    corners[i] = vec_add(corners[i], vec3(0, 0, -1));
  }

  Triangle faces[] = {
      {corners[0], corners[1], corners[2], colors[0], 0.0f},
      {corners[0], corners[2], corners[3], colors[0], 0.0f},
      {corners[4], corners[5], corners[6], colors[1], 0.0f},
      {corners[4], corners[6], corners[7], colors[1], 0.0f},
      {corners[0], corners[1], corners[5], colors[2], 0.0f},
      {corners[0], corners[5], corners[4], colors[2], 0.0f},
      {corners[2], corners[3], corners[7], colors[3], 0.0f},
      {corners[2], corners[7], corners[6], colors[3], 0.0f},
      {corners[0], corners[3], corners[7], colors[4], 0.0f},
      {corners[0], corners[7], corners[4], colors[4], 0.0f},
      {corners[1], corners[2], corners[6], colors[5], 0.0f},
      {corners[1], corners[6], corners[5], colors[5], 0.0f}
  };

  Triangle *ret = malloc(sizeof(faces));
  for (int i = 0; i < sizeof(faces) / sizeof(faces[0]); i++) {
    memcpy(&ret[i], &faces[i], sizeof(faces[0]));
  }

  (*n) = sizeof(faces) / sizeof(faces[0]);
  return ret;
}
