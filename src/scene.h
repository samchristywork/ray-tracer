#pragma once
#include "hittable.h"
#include "light.h"
#include <memory>
#include <vector>

struct Scene {
  std::vector<std::unique_ptr<Hittable>> objects;
  std::vector<PointLight> lights;

  void add(std::unique_ptr<Hittable> obj) { objects.push_back(std::move(obj)); }

  bool hit(const Ray &ray, double t_min, double t_max, HitRecord &rec) const {
    HitRecord tmp;
    bool hit_any = false;
    double closest = t_max;
    for (const auto &obj : objects) {
      if (obj->hit(ray, t_min, closest, tmp)) {
        hit_any = true;
        closest = tmp.t;
        rec = tmp;
      }
    }
    return hit_any;
  }
};
