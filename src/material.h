#pragma once
#include "vec3.h"
#include <cmath>

struct Material {
  Vec3 color = {1, 1, 1};
  double ambient = 0.1;
  double diffuse = 0.8;
  double specular = 0.5;
  double shininess = 32.0;
  double reflectivity = 0.0;
  bool checkerboard = false;
  double checker_scale = 1.0;

  Vec3 getColor(const Vec3 &point) const {
    if (!checkerboard)
      return color;
    int cx = (int)std::floor(point.x * checker_scale);
    int cz = (int)std::floor(point.z * checker_scale);
    return ((cx + cz) % 2 == 0) ? color : Vec3{0.05, 0.05, 0.05};
  }
};
