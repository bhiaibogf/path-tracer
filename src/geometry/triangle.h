//
// Created by bhiaibogf on 2022/2/21.
//

#ifndef PATH_TRACER_TRIANGLE_H
#define PATH_TRACER_TRIANGLE_H


#include "primitive.h"

class Triangle : public Primitive {
public:
    Triangle(Object *object,
             global::VectorArray *vertices, global::VectorArray *normals,
             global::TexCoordArray *tex_coords);

    ~Triangle() override = default;

    bool Intersect(Ray *ray, Intersection *intersection) const override;

    void Sample(Intersection *intersection, float *pdf) const override;

    void InsertTo(std::vector<Primitive *> *primitives) const override;

private:
    global::VectorArray vertices_, normals_;
    global::TexCoordArray tex_coords_;

};


#endif //PATH_TRACER_TRIANGLE_H
