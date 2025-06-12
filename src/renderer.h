#pragma once
#include "camera.h"
#include "ray.h"
#include "scene.h"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <vector>

static const int MAX_DEPTH = 6;

static Vec3 trace(const Ray &ray, const Scene &scene, int depth) {
  if (depth <= 0)
    return {0, 0, 0};

  HitRecord rec;
  if (!scene.hit(ray, 1e-4, 1e16, rec)) {
    // Sky gradient: white horizon -> light blue zenith
    double t = 0.5 * (ray.direction.y + 1.0);
    return Vec3{1, 1, 1} * (1 - t) + Vec3{0.45, 0.65, 1.0} * t;
  }

  const Material &mat = *rec.material;
  Vec3 color = mat.getColor(rec.point);

  // Ambient term
  Vec3 result = color * mat.ambient;

  for (const auto &light : scene.lights) {
    Vec3 to_light = light.position - rec.point;
    double light_dist = to_light.length();
    Vec3 light_dir = to_light / light_dist;

    // Shadow test
    Ray shadow{rec.point, light_dir};
    HitRecord shadow_rec;
    if (scene.hit(shadow, 1e-4, light_dist - 1e-4, shadow_rec))
      continue;

    double attenuation =
        light.intensity / (1.0 + 0.05 * light_dist * light_dist);

    // Diffuse (Lambertian)
    double diff = std::max(0.0, rec.normal.dot(light_dir));
    result += color * mat.diffuse * diff * light.color * attenuation;

    // Specular (Blinn-Phong)
    Vec3 view_dir = (-ray.direction).normalized();
    Vec3 half_dir = (light_dir + view_dir).normalized();
    double spec =
        std::pow(std::max(0.0, rec.normal.dot(half_dir)), mat.shininess);
    result += Vec3{1, 1, 1} * mat.specular * spec * light.color * attenuation;
  }

  // Reflection
  if (mat.reflectivity > 0.0 && depth > 1) {
    Vec3 reflect_dir = ray.direction.reflect(rec.normal).normalized();
    Ray reflect_ray{rec.point, reflect_dir};
    Vec3 reflect_color = trace(reflect_ray, scene, depth - 1);
    result =
        result * (1.0 - mat.reflectivity) + reflect_color * mat.reflectivity;
  }

  return clamp3(result, 0.0, 1.0);
}
