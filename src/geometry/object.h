//
// Created by bhiaibogf on 2022/2/21.
//

#ifndef PATH_TRACER_OBJECT_H
#define PATH_TRACER_OBJECT_H


#include "../utils/global.h"
#include "../material/material.h"
#include "../ray/ray.h"
#include "intersection.h"
#include "mesh.h"

class Object {
public:
    Object() = default;

    virtual ~Object() = default;

    auto material() const { return material_; }

    auto Area() const {
        return mesh_->area();
    }

    bool Intersect(Ray *ray, Intersection *intersection) const {
        intersection->material = material_;
        return mesh_->Intersect(ray, intersection);
    }

    void Sample(Intersection *intersection, float *pdf) const {
        return mesh_->Sample(intersection, pdf);
    }

private:
    Mesh *mesh_;
    Material *material_;

};


#endif //PATH_TRACER_OBJECT_H
