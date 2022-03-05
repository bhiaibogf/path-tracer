//
// Created by bhiaibogf on 2022/2/25.
//

#include "bvh.h"

Bvh::Bvh(const std::vector<Object *> &objects, SplitMethod split_method) {
    auto *primitives = new std::vector<const Primitive *>();
    for (auto object: objects) {
        object->InsertTo(primitives);
    }
    root_ = new Node(primitives, split_method);
}

bool Bvh::Intersect(Ray *ray, Intersection *intersection) const {
    return root_->Intersect(ray, intersection);
}

void Bvh::SampleLight(Intersection *intersection, float *pdf) const {
    float random_area = generator::Rand() * root_->area_weighted();
    root_->SampleLight(intersection, pdf, random_area);
    (*pdf) /= root_->area_weighted();
}
