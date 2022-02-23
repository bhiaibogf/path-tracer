//
// Created by bhiaibogf on 2022/2/21.
//

#include "triangle.h"

Triangle::Triangle(std::array<global::Vector, 3> vertices,
                   std::array<global::Vector, 3> normals,
                   std::array<global::TexCoord, 3> tex_coords) :
        vertices_(std::move(vertices)), normals_(std::move(normals)), tex_coords_(std::move(tex_coords)) {
    normal_ = (vertices_[1] - vertices_[0]).cross(vertices_[2] - vertices_[0]);
    area_ = normal_.norm() / 2;
    normal_.normalize();
    for (auto &normal: normals_) {
        normal = normal_;
    }

    // for (auto &vertex: vertices_) {
    //     std::cout << vertex << std::endl;
    // }
}

// Moller Trumbore
bool Triangle::Intersect(Ray *ray, Intersection *intersection) const {
    auto &v0 = vertices_[0], &v1 = vertices_[1], &v2 = vertices_[2];
    global::Vector edge1 = v1 - v0;
    global::Vector edge2 = v2 - v0;

    global::Vector pvec = ray->direction().cross(edge2);
    float det = edge1.dot(pvec);
    if (det <= 0) {
        return false;
    }

    global::Vector tvec = ray->origin() - v0;
    float u = tvec.dot(pvec);
    if (u < 0 || u > det) {
        return false;
    }

    global::Vector qvec = tvec.cross(edge1);
    float v = qvec.dot(ray->direction());
    if (v < 0 || u + v > det) {
        return false;
    }

    float inv_det = 1 / det;

    float t = edge2.dot(qvec) * inv_det;
    if (ray->Update(t)) {
        u *= inv_det;
        v *= inv_det;
        intersection->position = (*ray)(t);
        intersection->normal = (1 - u - v) * normals_[0] + u * normals_[1] + v * normals_[2];
        return true;
    }
    return false;
}

void Triangle::Sample(Intersection *intersection, float *pdf) const {
    // TODO
    float xi_1 = std::sqrt(global::Rand()), xi_2 = global::Rand();
    intersection->position =
            vertices_[0] * (1.f - xi_1) + vertices_[1] * (xi_1 * (1.f - xi_2)) + vertices_[2] * (xi_1 * xi_2);
    intersection->normal =
            normals_[0] * (1.f - xi_1) + normals_[1] * (xi_1 * (1.f - xi_2)) + normals_[2] * (xi_1 * xi_2);
    intersection->normal.normalize();
    *pdf = 1.f / area_;
}
