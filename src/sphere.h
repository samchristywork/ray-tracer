#pragma once
#include "hittable.h"
#include <cmath>

struct Sphere : Hittable {
  Vec3 center;
  double radius;
  Material material;

  Sphere(Vec3 center, double radius, Material mat)
      : center(center), radius(radius), material(mat) {}

  bool hit(const Ray &ray, double t_min, double t_max,
           HitRecord &rec) const override {
    Vec3 oc = ray.origin - center;
    double a = ray.direction.dot(ray.direction);
    double b = 2.0 * oc.dot(ray.direction);
    double c = oc.dot(oc) - radius * radius;
    double discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
      return false;

    double sqrtd = std::sqrt(discriminant);
    double t = (-b - sqrtd) / (2 * a);
    if (t < t_min || t > t_max) {
      t = (-b + sqrtd) / (2 * a);
      if (t < t_min || t > t_max)
        return false;
    }

    rec.t = t;
    rec.point = ray.at(t);
    rec.normal = (rec.point - center).normalized();
    rec.material = &material;
    return true;
  }
};
