#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define IMAGE_WIDTH 1600
#define IMAGE_HEIGHT 1600
#define MAX_RECURSION_DEPTH 10

typedef struct {
    float x, y, z;
} Vec3;

typedef struct {
    Vec3 center;
    float radius;
    Vec3 color;
    float reflection;
} Sphere;

typedef struct {
    Vec3 normal;
    float d;
    Vec3 color;
    float reflection;
} Plane;

typedef struct {
    Vec3 v0, v1, v2;
    Vec3 color;
    float reflection;
} Triangle;

Vec3 vec3(float x, float y, float z) {
    return (Vec3){x, y, z};
}

Vec3 vec_add(Vec3 a, Vec3 b) {
    return vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

Vec3 vec_sub(Vec3 a, Vec3 b) {
    return vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

Vec3 vec_scale(Vec3 a, float s) {
    return vec3(a.x * s, a.y * s, a.z * s);
}

float vec_dot(Vec3 a, Vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3 vec_norm(Vec3 a) {
    float mag = sqrtf(vec_dot(a, a));
    return vec_scale(a, 1.0f / mag);
}

Vec3 vec_cross(Vec3 a, Vec3 b) {
    return vec3(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

float clamp(float x, float lower, float upper) {
    if (x < lower) return lower;
    if (x > upper) return upper;
    return x;
}

int sphere_intersect(Sphere sphere, Vec3 origin, Vec3 direction, float *t) {
    Vec3 oc = vec_sub(origin, sphere.center);
    float a = vec_dot(direction, direction);
    float b = 2.0f * vec_dot(oc, direction);
    float c = vec_dot(oc, oc) - sphere.radius * sphere.radius;
    float discriminant = b * b - 4 * a * c;

    if (discriminant > 0) {
        float sqrtd = sqrtf(discriminant);
        float t0 = (-b - sqrtd) / (2.0f * a);
        float t1 = (-b + sqrtd) / (2.0f * a);
        *t = fminf(t0, t1);

        return (*t >= 0);
    }

    return 0;
}

int plane_intersect(Plane plane, Vec3 origin, Vec3 direction, float *t) {
    float denom = vec_dot(plane.normal, direction);
    if (fabs(denom) > 1e-6) { // Ensure this is not zero
        Vec3 p0l0 = vec_scale(plane.normal, plane.d);
        *t = vec_dot(p0l0, plane.normal) / denom;
        return (*t >= 0);
    }
    return 0;
}

float triangle_area(Vec3 a, Vec3 b, Vec3 c) {
    return vec_norm(vec_cross(vec_sub(b, a), vec_sub(c, a))).x / 2;
}

int triangle_intersect(Triangle triangle, Vec3 origin, Vec3 direction, float *t, Vec3 *hit_normal) {
    Vec3 edge1 = vec_sub(triangle.v1, triangle.v0);
    Vec3 edge2 = vec_sub(triangle.v2, triangle.v0);
    Vec3 h = vec_cross(direction, edge2);
    float a = vec_dot(edge1, h);
    if (a > -1e-6 && a < 1e-6)
        return 0; // This ray is parallel to this triangle.
    float f = 1.0f / a;
    Vec3 s = vec_sub(origin, triangle.v0);
    float u = f * vec_dot(s, h);
    if (u < 0.0 || u > 1.0)
        return 0;
    Vec3 q = vec_cross(s, edge1);
    float v = f * vec_dot(direction, q);
    if (v < 0.0 || u + v > 1.0)
        return 0;
    *t = f * vec_dot(edge2, q);
    if (*t > 1e-6) { // ray intersection
        *hit_normal = vec_norm(vec_cross(edge1, edge2));
        return 1;
    }
    return 0;
}

Vec3 trace_ray(Vec3 origin, Vec3 ray_direction, Sphere *spheres, int numSpheres, Plane *planes, int numPlanes, Triangle *triangles, int numTriangles, Vec3 *lights, int numLights, int depth) {
    Vec3 hit_color = vec3(0.0, 0.0, 0.3); // Default color
    if (depth >= MAX_RECURSION_DEPTH) {
        return hit_color;
    }

    float closest_t = FLT_MAX;
    int hit = 0;
    float hit_reflection = 0.0f;
    Vec3 normal, hit_point;
    Vec3 ambient_light = vec3(0.1f, 0.1f, 0.1f);

    // Sphere intersections
    for (int k = 0; k < numSpheres; ++k) {
        float t;
        if (sphere_intersect(spheres[k], origin, ray_direction, &t) && t < closest_t) {
            closest_t = t;
            hit = 1;
            hit_point = vec_add(origin, vec_scale(ray_direction, closest_t));
            normal = vec_norm(vec_sub(hit_point, spheres[k].center));
            hit_color = spheres[k].color;
            hit_reflection = spheres[k].reflection;
        }
    }

    if (hit) {
        float shade = 0;
        for (int i = 0; i < numLights; i++) {
            shade += fmax(vec_dot(normal, lights[i]), 0.0);
        }

        Vec3 reflection_direction = vec_norm(vec_sub(ray_direction, vec_scale(normal, 2 * vec_dot(ray_direction, normal))));
        Vec3 reflection_color = trace_ray(hit_point, reflection_direction, spheres, numSpheres, planes, numPlanes, triangles, numTriangles, lights, numLights, depth + 1);

        // Combine direct shading, reflection, and ambient light using the reflection coefficient
        Vec3 ambient_component = vec_scale(hit_color, 1.0f);
        Vec3 direct_component = vec_scale(hit_color, shade * (1.0f - hit_reflection));
        Vec3 reflected_component = vec_scale(reflection_color, hit_reflection);

        Vec3 final_color = vec_add(vec_add(direct_component, reflected_component), ambient_light);

        return final_color;
    } else {
        return hit_color;
    }
}

int main() {
    unsigned char *image = malloc(IMAGE_WIDTH * IMAGE_HEIGHT * 3);
    if (image == NULL) {
        fprintf(stderr, "Failed to allocate memory for the image\n");
        return 1;
    }

    free(image);
}
