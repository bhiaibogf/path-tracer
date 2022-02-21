//
// Created by bhiaibogf on 2022/1/23.
//

#include "scene.h"

Scene::Scene() {
    // TODO
}

Eigen::Vector3f Scene::Trace(const Ray &ray) const {
    Intersection intersection;
    if (Intersect(ray, &intersection)) {
        return Shade(intersection);
    }
    return kBackgroundColor;
}

bool Scene::Intersect(const Ray &ray, Intersection *intersection) const {
    intersection->direction = -ray.direction();
    // TODO
    return false;
}

Eigen::Vector3f Scene::Shade(const Intersection &intersection) const {
    Material *material = intersection.material;
    auto &position = intersection.position, &normal = intersection.normal, &direction = intersection.direction;

    // light
    Eigen::Vector3f radiance_light = kBlack;
    if (intersection.material->IsEmitter()) {
        radiance_light = material->emission();
    }

    // direct light
    Eigen::Vector3f radiance_direct = kBlack;
    // sample from light
    Intersection intersection_light;
    float pdf_light;
    SampleLight(&intersection_light, &pdf_light);

    auto &position_light = intersection_light.position, &normal_light = intersection_light.normal;
    auto emission_light = intersection.material->emission();
    // wi (inter to light)
    auto direction_to_light = (position_light - position).normalized();

    // check if the light is visible
    Intersection intersection_to_light;
    if (Intersect(Ray(position, direction_to_light), &intersection_to_light)
        && (position_light - intersection_to_light.position).squaredNorm() < kEpsilon) {
        radiance_direct = emission_light
                          * material->Eval(direction, direction_to_light, normal)
                          * normal.dot(direction_to_light)
                          // TODO
                          * normal_light.dot(-direction_to_light)
                          / (position_light - position).squaredNorm()
                          / pdf_light;
    }

    // indirect light
    Eigen::Vector3f radiance_indirect = kBlack;
    if (RussianRoulette()) {
        // wi (inter to next)
        auto direction_to_next = material->Sample(direction, normal);
        Ray ray_to_next = Ray(position, direction_to_next);
        Intersection intersection_next;
        if (Intersect(ray_to_next, &intersection_next)) {
            radiance_indirect = Shade(intersection_next)
                                * material->Eval(direction, direction_to_next, normal)
                                * normal.dot(direction_to_next)
                                / material->Pdf(direction, direction_to_next, normal)
                                / kRussianRoulette;
        }
    }

    return radiance_light + radiance_direct + radiance_indirect;
}

bool Scene::RussianRoulette() const {
    return funcs::RandomFloat() < kRussianRoulette;
}

void Scene::SampleLight(Intersection *intersection, float *pdf) const {
    // TODO
}
