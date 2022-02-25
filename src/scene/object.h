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
    Object();

    virtual ~Object() = default;

    void SetMesh(Mesh *mesh);

    void SetMaterial(Material *material);

    auto material() const { return material_; }

    void InsertTo(std::vector<Primitive *> *primitives) const;

    bool Intersect(Ray *ray, Intersection *intersection) const;

    auto Area() const { return mesh_->area(); }

    void Sample(Intersection *intersection, float *pdf) const;

private:
    Mesh *mesh_;
    Material *material_;

};


#endif //PATH_TRACER_OBJECT_H