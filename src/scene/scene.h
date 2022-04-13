//
// Created by bhiaibogf on 2022/1/23.
//

#ifndef PATH_TRACER_SCENE_H
#define PATH_TRACER_SCENE_H


#include "alias_table.h"
#include "../bvh/bvh.h"
#include "../material/skybox.h"

class Scene {
public:
    Scene();

    ~Scene() = default;

    void SetSkybox(Skybox *skybox) { skybox_ = skybox; }

    void AddObject(Object *object);

    void BuildBvh();

    enum ShadingType {
        kUv,
        kAlbedo,
        kNormal,
        kPosition,
        kDepth,

        kSampleLight,
        kSampleBsdf,
        kMis
    };

    global::Color Trace(Ray *ray, ShadingType shading_type) const;

private:
    static const global::Color kBackgroundColor;
    static const float kEpsilonPosition;
    static const float kEpsilonLight;
    static const float kEpsilonPdf;
    static const float kRussianRoulette;
    static const int kMaxBounce;

    std::vector<Object *> objects_;
    Skybox *skybox_;

    Bvh *bvh_;
    AliasTable *alias_table_;

    Bound bound_;
    float area_weighted_sum_;
    float scale_;

    bool Intersect(Ray *ray, Intersection *intersection) const;

    global::Color Shade(const Ray &ray, const Intersection &intersection, ShadingType g_buffer_type) const;

    global::Color Shade(const Intersection &intersection, int bounce, ShadingType sample_type) const;

    std::pair<global::Vector, global::Vector> SampleLight(const global::Vector &position, float *pdf) const;

    float PdfLight(const global::Vector &position, const Intersection &intersection_another_light) const;

    static bool RussianRoulette(int bounce);

};


#endif //PATH_TRACER_SCENE_H
