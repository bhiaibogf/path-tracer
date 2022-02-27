//
// Created by bhiaibogf on 2022/1/23.
//

#ifndef PATH_TRACER_SCENE_H
#define PATH_TRACER_SCENE_H


#include "object.h"
#include "../bvh/bvh.h"

class Scene {
public:
    Scene();

    ~Scene() = default;

    void AddObject(Object *object) { objects_.push_back(object); }

    void BuildBvh();

    enum SampleType {
        kSampleLight, kSampleBsdf, kSampleBoth, kMis
    };

    global::Color Trace(Ray *ray, SampleType sample_type) const;

private:
    static const Eigen::Vector3f kBackgroundColor;
    static const float kEpsilon;
    static const float kRussianRoulette;
    static const int kMaxBounce;

    std::vector<Object *> objects_;
    Bvh *bvh_;

    bool Intersect(Ray *ray, Intersection *intersection) const;

    global::Color Shade(const Intersection &intersection, int bounce) const;

    global::Color Shade(const Intersection &intersection, int bounce, SampleType sample_type) const;

    void SampleLight(Intersection *intersection, float *pdf) const;

    static bool RussianRoulette(int bounce);

};


#endif //PATH_TRACER_SCENE_H
