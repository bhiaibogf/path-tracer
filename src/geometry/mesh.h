//
// Created by bhiaibogf on 2022/1/23.
//

#ifndef PATH_TRACER_MESH_H
#define PATH_TRACER_MESH_H


#include "triangle.h"

class Mesh : public Primitive {
public:
    explicit Mesh(Object *object) : Primitive(object) {}

    ~Mesh() override = default;

    PrimitiveType Type() const override { return kMesh; }

    void Add(Triangle *triangle);

    bool Intersect(Ray *ray, Intersection *intersection) const override;

    void Sample(Intersection *intersection, float *pdf) const override;

    void InsertTo(std::vector<const Primitive *> *primitives) const override;

    friend std::ostream &operator<<(std::ostream &os, const Mesh &mesh);

private:
    std::vector<Triangle *> triangles_;

};


#endif //PATH_TRACER_MESH_H
