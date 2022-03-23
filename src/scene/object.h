//
// Created by bhiaibogf on 2022/2/21.
//

#ifndef PATH_TRACER_OBJECT_H
#define PATH_TRACER_OBJECT_H


#include "../geometry/mesh.h"
#include "../geometry/sphere.h"
#include "../material/mix.h"

class Object {
public:
    explicit Object(std::string name);

    virtual ~Object() = default;

    auto name() const { return name_; }

    void SetPrimitive(Primitive *primitive);

    void SetMaterial(Material *material);

    auto material() const { return material_; }

    auto primitive() const { return primitive_; }

    auto AreaWeighted() const { return primitive_->AreaWeighted(); }

    void InsertTo(std::vector<const Primitive *> *primitives) const;

    bool Intersect(Ray *ray, Intersection *intersection) const;

    void Sample(Intersection *intersection, float *pdf) const;

private:
    Primitive *primitive_;
    Material *material_;
    std::string name_;

};


#endif //PATH_TRACER_OBJECT_H
