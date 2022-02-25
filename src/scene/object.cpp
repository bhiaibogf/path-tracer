//
// Created by bhiaibogf on 2022/2/21.
//

#include "object.h"

bool Object::Intersect(Ray *ray, Intersection *intersection) const {
    if (mesh_->Intersect(ray, intersection)) {
        intersection->material = material_;
        return true;
    }
    return false;
}

void Object::Sample(Intersection *intersection, float *pdf) const {
    return mesh_->Sample(intersection, pdf);
}
