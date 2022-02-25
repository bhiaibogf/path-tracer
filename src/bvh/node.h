//
// Created by bhiaibogf on 2022/2/25.
//

#ifndef PATH_TRACER_NODE_H
#define PATH_TRACER_NODE_H


#include "bound.h"
#include "../geometry/primitive.h"
#include "../ray/ray.h"

class Node {
public:
    explicit Node(std::vector<Primitive *> primitives);

    bool Intersect(Ray *ray, Intersection *intersection) const;

private:
    Bound bound;
    Primitive *primitive;
    float area;

    Node *left;
    Node *right;

};


#endif //PATH_TRACER_NODE_H
