//
// Created by bhiaibogf on 2022/2/25.
//

#ifndef PATH_TRACER_BVH_H
#define PATH_TRACER_BVH_H


#include "node.h"
#include "../scene/object.h"

class Bvh {
public:
    explicit Bvh(const std::vector<Object *> &objects, SplitMethod split_method = kMix);

    ~Bvh() = default;

    float AreaWeighted() const { return root_->area_weighted(); }

    float Diagonal() const { return root_->bound().Diagonal().norm(); }

    bool Intersect(Ray *ray, Intersection *intersection) const;

    void SampleLight(Intersection *intersection, float *pdf) const;

private:
    Node *root_;

};


#endif //PATH_TRACER_BVH_H
