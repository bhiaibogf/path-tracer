//
// Created by bhiaibogf on 2022/1/23.
//

#include "mesh.h"

bool Mesh::Intersect(Ray *ray, Intersection *intersection) const {
    bool has_intersection = false;
    for (const auto &triangle: triangles_) {
        if (triangle->Intersect(ray, intersection)) {
            has_intersection = true;
        }
    }
    return has_intersection;
}

void Mesh::Sample(Intersection *intersection, float *pdf) const {
    float random_area = global::RandomFloat() * area_;
    float area_sum = 0;
    for (const auto &triangle: triangles_) {
        area_sum += triangle->area();
        if (random_area <= area_sum) {
            triangle->Sample(intersection, pdf);
            break;
        }
    }
}

void Mesh::Add(Triangle *triangle) {
    triangles_.push_back(triangle);
    area_ += triangle->area();
}

std::ostream &operator<<(std::ostream &os, const Mesh &mesh) {
    os << "Mesh: " << mesh.triangles_.size() << " triangles" << " " << mesh.area_;
    return os;
}
