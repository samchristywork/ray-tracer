#pragma once
#include "ray.h"
#include <cmath>

struct Camera {
  Vec3 origin;
  Vec3 lower_left;
  Vec3 horizontal;
  Vec3 vertical;

  Camera(Vec3 eye, Vec3 target, Vec3 up, double vfov_deg, double aspect) {
    double theta = vfov_deg * M_PI / 180.0;
    double vp_height = 2.0 * std::tan(theta / 2.0);
    double vp_width = aspect * vp_height;

    Vec3 w = (eye - target).normalized();
    Vec3 u = up.cross(w).normalized();
    Vec3 v = w.cross(u);

    origin = eye;
    horizontal = u * vp_width;
    vertical = v * vp_height;
    lower_left = origin - horizontal / 2 - vertical / 2 - w;
  }

  Ray getRay(double s, double t) const {
    Vec3 dir = lower_left + horizontal * s + vertical * t - origin;
    return {origin, dir.normalized()};
  }
};
