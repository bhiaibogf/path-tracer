//
// Created by bhiaibogf on 2022/2/25.
//

#include "bvh.h"

Bvh::Bvh(const std::vector<Object *> &objects) {
    auto *primitives = new std::vector<Primitive *>();
    for (auto object: objects) {
        object->InsertTo(primitives);
    }
    root_ = new Node(primitives);
}

bool Bvh::Intersect(Ray *ray, Intersection *intersection) const {
    return root_->Intersect(ray, intersection);
}