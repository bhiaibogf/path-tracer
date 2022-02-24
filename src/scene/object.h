//
// Created by bhiaibogf on 2022/2/21.
//

#ifndef PATH_TRACER_OBJECT_H
#define PATH_TRACER_OBJECT_H


#include "../geometry/mesh.h"
#include "../material/lambert.h"
#include "../material/phong.h"
#include "../material/refraction.h"

class Object {
public:
    Object(Mesh *mesh, Material *material);

    virtual ~Object() = default;

    auto material() const { return material_; }

    auto Area() const {
        return mesh_->area();
    }

    bool Intersect(Ray *ray, Intersection *intersection) const;

    void Sample(Intersection *intersection, float *pdf) const;

private:
    Mesh *mesh_;
    Material *material_;

};


#endif //PATH_TRACER_OBJECT_H
