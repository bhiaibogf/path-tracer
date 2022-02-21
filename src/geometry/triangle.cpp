//
// Created by bhiaibogf on 2022/2/21.
//

#include "triangle.h"

bool Triangle::Intersect(Ray *ray, Intersection *intersection) const {
    auto &v0 = vertices_[0], &v1 = vertices_[1], &v2 = vertices_[2];
    global::Vector edge1 = v1 - v0;
    global::Vector edge2 = v2 - v0;

    global::Vector pvec = edge2.cross(ray->direction());
    float det = edge1.dot(pvec);
    if (det == 0 || det < 0) {
        return false;
    }

    global::Vector tvec = ray->origin() - v0;
    float u = tvec.dot(pvec);
    if (u < 0 || u > det) {
        return false;
    }

    global::Vector qvec = tvec.cross(edge1);
    float v = qvec.dot(ray->origin());
    if (v < 0 || u + v > det) {
        return false;
    }

    float inv_det = 1 / det;

    float t = edge2.dot(qvec) * inv_det;
    if (ray->Update(t)) {
        u *= inv_det;
        v *= inv_det;
        intersection->position = (*ray)(t);
        // TODO
        intersection->normal = u * normals_[2] + v * normals_[1] + (1 - u - v) * normals_[0];
        return true;
    }
    return false;
}

void Triangle::Sample(Intersection *intersection, float *pdf) const {
    // TODO
    float xi_1 = std::sqrt(global::RandomFloat()), xi_2 = global::RandomFloat();
    intersection->position =
            vertices_[0] * (1.f - xi_1) + vertices_[1] * (xi_1 * (1.f - xi_2)) + vertices_[2] * (xi_1 * xi_2);
    intersection->normal =
            normals_[0] * (1.f - xi_1) + normals_[1] * (xi_1 * (1.f - xi_2)) + normals_[2] * (xi_1 * xi_2);
    intersection->normal.normalize();
    *pdf = 1.f / area_;
}
