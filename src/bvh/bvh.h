//
// Created by bhiaibogf on 2022/2/25.
//

#ifndef PATH_TRACER_BVH_H
#define PATH_TRACER_BVH_H


#include "node.h"
#include "../scene/object.h"

class Bvh {
public:
    explicit Bvh(const std::vector<Object *> &objects);

    ~Bvh() = default;

    bool Intersect(Ray *ray, Intersection *intersection) const;

private:
    Node *root_;

};


#endif //PATH_TRACER_BVH_H
