//
// Created by bhiaibogf on 2022/1/23.
//

#include "scene.h"

const Eigen::Vector3f Scene::kBackgroundColor = global::Color(0.2f, 0.4f, 0.8f);
const float Scene::kEpsilon = 1e-6f;
const float Scene::kRussianRoulette = 0.8f;
const int Scene::kMaxBounce = 10;

global::Color Scene::Trace(Ray *ray) const {
    Intersection intersection;
    if (Intersect(ray, &intersection)) {
        return Shade(intersection, 1);
    }
    return kBackgroundColor;
}

bool Scene::Intersect(Ray *ray, Intersection *intersection) const {
    intersection->direction = -ray->direction();
    return bvh_->Intersect(ray, intersection);
}

global::Color Scene::Shade(const Intersection &intersection, int bounce) const {
    if (bounce > kMaxBounce) {
        return global::kBlack;
    }

    Material *material = intersection.material;
    auto &position = intersection.position, &normal = intersection.normal, &direction = intersection.direction;
    auto &tex_coord = intersection.tex_coord;

    // light
    global::Color radiance_light = global::kBlack;
    if (material->HasEmitter()) {
        radiance_light = material->emission();
    }

    // direct light
    global::Color radiance_direct = global::kBlack;
    // sample from light
    Intersection intersection_light;
    float pdf_light;
    SampleLight(&intersection_light, &pdf_light);

    auto &position_light = intersection_light.position, &normal_light = intersection_light.normal;
    // wi (inter to light)
    auto direction_to_light = (position_light - position).normalized();

    // check if the light is visible
    Intersection intersection_to_light;
    Ray ray_to_light(position, direction_to_light);
    if (Intersect(&ray_to_light, &intersection_to_light)
        && (position_light - intersection_to_light.position).squaredNorm() < kEpsilon) {
        radiance_direct = global::Product(Shade(intersection_to_light, bounce + 1),
                                          material->Eval(direction, direction_to_light, normal, tex_coord))
                          * normal.dot(direction_to_light)
                          // TODO
                          * normal_light.dot(-direction_to_light)
                          / (position_light - position).squaredNorm()
                          / pdf_light;
    }

    // indirect light
    global::Color radiance_indirect = global::kBlack;
    if (RussianRoulette(bounce)) {
        // wi (inter to next)
        auto direction_to_next = material->Sample(direction, normal);
        global::Vector position_new = position + normal * (normal.dot(direction_to_next) > 0 ? kEpsilon : -kEpsilon);
        // global::Vector position_new = position;
        Ray ray_to_next = Ray(position_new, direction_to_next);
        Intersection intersection_next;
        if (Intersect(&ray_to_next, &intersection_next)) {
            radiance_indirect = global::Product(Shade(intersection_next, bounce + 1),
                                                material->Eval(direction, direction_to_next, normal, tex_coord))
                                * std::abs(normal.dot(direction_to_next))
                                / material->Pdf(direction, direction_to_next, normal)
                                / kRussianRoulette;
        }
    }

    return radiance_light + 0.5 * radiance_direct + 0.5 * radiance_indirect;
}

bool Scene::RussianRoulette(int bounce) {
    return generator::Rand() < kRussianRoulette;
}

void Scene::SampleLight(Intersection *intersection, float *pdf) const {
    float area_sum = 0;
    for (const auto &object: objects_) {
        if (object->material()->HasEmitter()) {
            area_sum += object->Area();
        }
    }

    float random_area = generator::Rand() * area_sum;
    area_sum = 0;
    for (const auto &object: objects_) {
        if (object->material()->HasEmitter()) {
            area_sum += object->Area();
            if (random_area <= area_sum) {
                intersection->material = object->material();
                object->Sample(intersection, pdf);
                break;
            }
        }
    }
}

void Scene::BuildBvh() {
    bvh_ = new Bvh(objects_);
}
