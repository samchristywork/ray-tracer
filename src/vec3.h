#pragma once
#include <algorithm>
#include <cmath>

struct Vec3 {
  double x = 0, y = 0, z = 0;

  Vec3() = default;
  Vec3(double x, double y, double z) : x(x), y(y), z(z) {}

  Vec3 operator+(const Vec3 &o) const { return {x + o.x, y + o.y, z + o.z}; }
  Vec3 &operator+=(const Vec3 &o) {
    x += o.x;
    y += o.y;
    z += o.z;
    return *this;
  }
  Vec3 operator-(const Vec3 &o) const { return {x - o.x, y - o.y, z - o.z}; }
  Vec3 operator*(double t) const { return {x * t, y * t, z * t}; }
  Vec3 operator*(const Vec3 &o) const { return {x * o.x, y * o.y, z * o.z}; }
  Vec3 operator/(double t) const { return {x / t, y / t, z / t}; }
  Vec3 operator-() const { return {-x, -y, -z}; }

  double dot(const Vec3 &o) const { return x * o.x + y * o.y + z * o.z; }
  Vec3 cross(const Vec3 &o) const {
    return {y * o.z - z * o.y, z * o.x - x * o.z, x * o.y - y * o.x};
  }
  double length() const { return std::sqrt(x * x + y * y + z * z); }
  Vec3 normalized() const {
    double l = length();
    return l > 1e-12 ? *this / l : Vec3{0, 0, 0};
  }
  Vec3 reflect(const Vec3 &n) const { return *this - n * (2.0 * dot(n)); }
};

inline Vec3 operator*(double t, const Vec3 &v) { return v * t; }

inline Vec3 clamp3(const Vec3 &v, double lo, double hi) {
  return {
      std::clamp(v.x, lo, hi),
      std::clamp(v.y, lo, hi),
      std::clamp(v.z, lo, hi),
  };
}
