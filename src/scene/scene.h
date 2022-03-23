//
// Created by bhiaibogf on 2022/1/23.
//

#ifndef PATH_TRACER_SCENE_H
#define PATH_TRACER_SCENE_H


#include "alias_table.h"
#include "../bvh/bvh.h"

class Scene {
public:
    Scene();

    ~Scene() = default;

    void AddObject(Object *object) {
        objects_.push_back(object);
        bound_ |= object->primitive()->bound();
        if (object->material()->HasEmitter()) {
            area_weighted_sum_ += object->AreaWeighted();
        }
        scale_ = bound_.Diagonal().norm();
    }

    void BuildBvh();

    enum SampleType {
        kSampleLight, kSampleBsdf, kSampleBoth, kMis
    };

    global::Color Trace(Ray *ray, SampleType sample_type) const;

private:
    static const global::Color kBackgroundColor;
    static const float kEpsilonPosition;
    static const float kEpsilonLight;
    static const float kEpsilonPdf;
    static const float kRussianRoulette;
    static const int kMaxBounce;

    std::vector<Object *> objects_;

    Bvh *bvh_;
    AliasTable *alias_table_;

    Bound bound_;
    float area_weighted_sum_;
    float scale_;

    bool Intersect(Ray *ray, Intersection *intersection) const;

    global::Color Shade(const Intersection &intersection, int bounce, SampleType sample_type) const;

    std::pair<global::Vector, global::Vector> SampleLight(const global::Vector &position, float *pdf) const;

    float PdfLight(const global::Vector &position, const Intersection &intersection_another_light) const;

    static bool RussianRoulette(int bounce);

};


#endif //PATH_TRACER_SCENE_H
