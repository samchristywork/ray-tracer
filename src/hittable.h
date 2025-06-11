#pragma once
#include "material.h"
#include "ray.h"

struct HitRecord {
  double t;
  Vec3 point;
  Vec3 normal;
  const Material *material;
};

struct Hittable {
  virtual ~Hittable() = default;
  virtual bool hit(const Ray &ray, double t_min, double t_max,
                   HitRecord &rec) const = 0;
};
