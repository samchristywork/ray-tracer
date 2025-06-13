#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

#include "plane.h"
#include "renderer.h"
#include "sphere.h"

#include <cstdio>
#include <memory>

int main() {
  const int WIDTH = 800;
  const int HEIGHT = 600;

  // Camera: elevated and slightly back, looking at scene center
  Camera camera({0, 3.5, 3},  // eye
                {0, 0.8, -4}, // target
                {0, 1, 0},    // up
                55.0,         // vertical fov (degrees)
                (double)WIDTH / HEIGHT);

  Scene scene;

  // --- Ground plane: reflective checkerboard ---
  Material ground;
  ground.color = {0.85, 0.85, 0.85};
  ground.ambient = 0.08;
  ground.diffuse = 0.7;
  ground.specular = 0.15;
  ground.shininess = 8;
  ground.reflectivity = 0.2;
  ground.checkerboard = true;
  ground.checker_scale = 0.4;
  scene.add(std::make_unique<Plane>(Vec3{0, 0, 0}, Vec3{0, 1, 0}, ground));

  // --- Red matte sphere (left) ---
  Material red;
  red.color = {0.88, 0.18, 0.18};
  red.diffuse = 0.85;
  red.specular = 0.25;
  red.shininess = 20;
  scene.add(std::make_unique<Sphere>(Vec3{-2.5, 1, -5}, 1.0, red));

  // --- Blue glossy sphere (center-back) ---
  Material blue;
  blue.color = {0.15, 0.25, 0.90};
  blue.diffuse = 0.6;
  blue.specular = 0.9;
  blue.shininess = 80;
  scene.add(std::make_unique<Sphere>(Vec3{0, 1, -5.5}, 1.0, blue));

  // --- Green matte sphere (right) ---
  Material green;
  green.color = {0.15, 0.78, 0.25};
  green.diffuse = 0.85;
  green.specular = 0.2;
  green.shininess = 16;
  scene.add(std::make_unique<Sphere>(Vec3{2.5, 1, -5}, 1.0, green));

  // --- Mirror sphere (front-center) ---
  Material mirror;
  mirror.color = {0.95, 0.95, 0.95};
  mirror.ambient = 0.03;
  mirror.diffuse = 0.05;
  mirror.specular = 1.0;
  mirror.shininess = 512;
  mirror.reflectivity = 0.92;
  scene.add(std::make_unique<Sphere>(Vec3{0, 0.6, -2.5}, 0.6, mirror));

  // --- Gold sphere (right-front) ---
  Material gold;
  gold.color = {1.0, 0.72, 0.08};
  gold.ambient = 0.08;
  gold.diffuse = 0.6;
  gold.specular = 1.0;
  gold.shininess = 128;
  gold.reflectivity = 0.35;
  scene.add(std::make_unique<Sphere>(Vec3{1.7, 0.5, -3.2}, 0.5, gold));

  // --- Small purple sphere (left-front) ---
  Material purple;
  purple.color = {0.6, 0.1, 0.8};
  purple.diffuse = 0.7;
  purple.specular = 0.6;
  purple.shininess = 48;
  purple.reflectivity = 0.1;
  scene.add(std::make_unique<Sphere>(Vec3{-1.5, 0.4, -3.0}, 0.4, purple));

  // --- Lights ---
  // Warm key light (upper-left)
  scene.lights.push_back({{-4, 7, 3}, {1.0, 0.92, 0.80}, 1.2});
  // Cool fill light (upper-right)
  scene.lights.push_back({{5, 4, 0}, {0.55, 0.65, 1.0}, 0.7});
  // Weak back light for rim effect
  scene.lights.push_back({{0, 3, -10}, {0.8, 0.85, 1.0}, 0.4});

  // --- Render ---
  std::vector<uint8_t> pixels;
  render(camera, scene, WIDTH, HEIGHT, pixels);

  if (!stbi_write_png("output.png", WIDTH, HEIGHT, 3, pixels.data(),
                      WIDTH * 3)) {
    fprintf(stderr, "Failed to write output.png\n");
    return 1;
  }
  printf("Rendered %dx%d -> output.png\n", WIDTH, HEIGHT);
  return 0;
}
