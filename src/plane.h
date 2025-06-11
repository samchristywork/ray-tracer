#pragma once
#include "hittable.h"
#include <cmath>

struct Plane : Hittable {
  Vec3 point;
  Vec3 normal;
  Material material;

  Plane(Vec3 point, Vec3 normal, Material mat)
      : point(point), normal(normal.normalized()), material(mat) {}

  bool hit(const Ray &ray, double t_min, double t_max,
           HitRecord &rec) const override {
    double denom = normal.dot(ray.direction);
    if (std::abs(denom) < 1e-8)
      return false;

    double t = (point - ray.origin).dot(normal) / denom;
    if (t < t_min || t > t_max)
      return false;

    rec.t = t;
    rec.point = ray.at(t);
    rec.normal = denom < 0 ? normal : -normal;
    rec.material = &material;
    return true;
  }
};
