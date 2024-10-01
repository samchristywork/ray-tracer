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

    // Plane intersections
    for (int p = 0; p < numPlanes; ++p) {
        float t;
        if (plane_intersect(planes[p], origin, ray_direction, &t) && t < closest_t) {
            closest_t = t;
            hit = 1;
            hit_point = vec_add(origin, vec_scale(ray_direction, closest_t));
            normal = planes[p].normal;
            hit_color = planes[p].color;
            hit_reflection = planes[p].reflection;
        }
    }

    // Triangle intersections
    for (int t = 0; t < numTriangles; ++t) {
        Vec3 tri_normal;
        float tri_t;
        if (triangle_intersect(triangles[t], origin, ray_direction, &tri_t, &tri_normal) && tri_t < closest_t) {
            closest_t = tri_t;
            hit = 1;
            hit_point = vec_add(origin, vec_scale(ray_direction, closest_t));
            normal = tri_normal;
            hit_color = triangles[t].color;
            hit_reflection = triangles[t].reflection;
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

void render_scene(unsigned char *image) {
    // Define scene objects and lights
    Sphere spheres[] = {
        {vec3(-1, -0.25, -1), 0.25f, vec3(1.0f, 0.0f, 0.0f), 0.5f},
        {vec3(-1, -0.25, -2), 0.25f, vec3(0.0f, 1.0f, 0.0f), 0.5f},
        {vec3(1, 1, -2), 1.0f, vec3(0.0f, 0.0f, 1.0f), 0.5f}
    };
    int numSpheres = sizeof(spheres) / sizeof(spheres[0]);

    Plane planes[] = {
        {vec3(0, 1, 0), -0.5f, vec3(0.1f, 0.1f, 0.1f), 0.1f},
    };
    int numPlanes = sizeof(planes) / sizeof(planes[0]);

    // Why isn't the triangle being colored correctly?
    Triangle triangles[] = {
        {
          .v0=vec3(0.25, 0.5, -1),
          .v1=vec3(0.0, 0.5, -1),
          .v2=vec3(0.0, 0.0, -1),
          .color=vec3(1.0, 0.0, 1.0),
          .reflection=0.0f
        }
    };
    int numTriangles = sizeof(triangles) / sizeof(triangles[0]);

    // Camera parameters
    Vec3 origin = vec3(0.0f, 0.0f, 1.0f);
    Vec3 look_at = vec3(0.0f, 0.0f, 0.0f);
    Vec3 up = vec3(0.0f, 1.0f, 0.0f);
    Vec3 w = vec_norm(vec_sub(origin, look_at));
    Vec3 u = vec_norm(vec_cross(up, w));
    Vec3 v = vec_cross(w, u);
    float viewport_height = 2.0f;
    float viewport_width = 2.0f;
    float focal_length = 1.0f;

    Vec3 horizontal = vec_scale(u, viewport_width);
    Vec3 vertical = vec_scale(v, viewport_height);
    Vec3 lower_left_corner = vec_sub(vec_sub(vec_sub(origin, vec_scale(horizontal, 0.5f)), vec_scale(vertical, 0.5f)), w);

    // Light sources
    Vec3 lights[] = {
        vec3(-0.8f, 0.8f, 0.2f),
        vec3(0.8f, 0.8f, 0.2f)
    };
    int numLights = sizeof(lights) / sizeof(lights[0]);

    for (int j = 0; j < IMAGE_HEIGHT; ++j) {
        for (int i = 0; i < IMAGE_WIDTH; ++i) {
            float u = (float)i / (float)(IMAGE_WIDTH - 1);
            float v = (float)j / (float)(IMAGE_HEIGHT - 1);
            Vec3 direction = vec_add(lower_left_corner, vec_add(vec_scale(horizontal, u), vec_scale(vertical, v)));
            Vec3 ray_direction = vec_norm(vec_sub(direction, origin));

            int pixel_index = ((IMAGE_HEIGHT - 1 - j) * IMAGE_WIDTH + i) * 3;

            Vec3 color = trace_ray(origin, ray_direction, spheres, numSpheres, planes, numPlanes, triangles, numTriangles, lights, numLights, 0);

            color.x *= 254.0f;
            color.y *= 254.0f;
            color.z *= 254.0f;

            image[pixel_index + 0] = (unsigned char)color.x;
            image[pixel_index + 1] = (unsigned char)color.y;
            image[pixel_index + 2] = (unsigned char)color.z;
        }
    }
}

int main() {
    unsigned char *image = malloc(IMAGE_WIDTH * IMAGE_HEIGHT * 3);
    if (image == NULL) {
        fprintf(stderr, "Failed to allocate memory for the image\n");
        return 1;
    }

    render_scene(image);

    free(image);
}
