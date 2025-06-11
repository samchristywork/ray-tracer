#pragma once
#include "vec3.h"

struct PointLight {
  Vec3 position;
  Vec3 color;
  double intensity;
};
