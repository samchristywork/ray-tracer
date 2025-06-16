#pragma once
#include "camera.h"
#include "ray.h"
#include "scene.h"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <random>
#include <vector>


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

inline void render(const Camera &camera, const Scene &scene, int width,
                   int height, std::vector<uint8_t> &pixels,
                   int samples = 16, int max_depth = 6) {
  pixels.resize(width * height * 3);
  std::mt19937 rng(42);
  std::uniform_real_distribution<double> dist(0.0, 1.0);
  for (int j = 0; j < height; j++) {
    int pct = (j * 100) / height;
    printf("\rRendering: %3d%%  [%.*s%*s]", pct,
           pct / 5, "####################",
           20 - pct / 5, "");
    fflush(stdout);
    for (int i = 0; i < width; i++) {
      Vec3 color = {0, 0, 0};
      for (int s = 0; s < samples; s++) {
        double u = (i + dist(rng)) / width;
        double v = (height - 1 - j + dist(rng)) / height;
        Ray ray = camera.getRay(u, v);
        color += trace(ray, scene, max_depth);
      }
      color = clamp3(color * (1.0 / samples), 0.0, 1.0);
      int idx = (j * width + i) * 3;
      pixels[idx + 0] = (uint8_t)(color.x * 255.99);
      pixels[idx + 1] = (uint8_t)(color.y * 255.99);
      pixels[idx + 2] = (uint8_t)(color.z * 255.99);
    }
  }
  printf("\rRendering: 100%%  [####################]\n");
}
