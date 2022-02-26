//
// Created by bhiaibogf on 2022/1/23.
//

#include "scene.h"

const Eigen::Vector3f Scene::kBackgroundColor = global::Color(0.2f, 0.4f, 0.8f);
const float Scene::kEpsilon = 1e-6f;
const float Scene::kRussianRoulette = 0.8f;
const int Scene::kMaxBounce = 10;

Scene::Scene() {
    bvh_ = nullptr;
}

void Scene::BuildBvh() {
    bvh_ = new Bvh(objects_);
}

global::Color Scene::Trace(Ray *ray, bool sample_to_light) const {
    Intersection intersection;
    if (Intersect(ray, &intersection)) {
        if (sample_to_light) {
            return Shade(intersection, 0, true);
        } else {
            return ShadeMis(intersection, 0);
        }
    }
    return kBackgroundColor;
}

bool Scene::Intersect(Ray *ray, Intersection *intersection) const {
    intersection->direction = -ray->direction();
    if (bvh_) {
        return bvh_->Intersect(ray, intersection);
    } else {
        bool has_intersection = false;
        for (auto &object: objects_) {
            if (object->Intersect(ray, intersection)) {
                has_intersection = true;
            }
        }
        return has_intersection;
    }
}

global::Color Scene::Shade(const Intersection &intersection, int bounce) const {
    Material *material = intersection.material;
    auto &position = intersection.position, &normal = intersection.normal, &direction = intersection.direction;
    auto &tex_coord = intersection.tex_coord;

    // light
    global::Color radiance_light = global::kBlack;
    if (material->HasEmitter()) {
        radiance_light = material->emission();
    }

    if (bounce >= kMaxBounce) {
        return radiance_light;
    }

    // other
    global::Color radiance_path = global::kBlack;
    if (RussianRoulette(bounce)) {
        // wi (inter to next)
        auto direction_to_next = material->Sample(direction, normal);
        if (direction_to_next == global::kNone) {
            return radiance_light;
        }
        global::Vector position_new = position + normal * (normal.dot(direction_to_next) > 0 ? kEpsilon : -kEpsilon);
        // global::Vector position_new = position;
        Ray ray_to_next = Ray(position_new, direction_to_next);
        Intersection intersection_next;
        if (Intersect(&ray_to_next, &intersection_next)) {
            radiance_path = global::Product(Shade(intersection_next, bounce + 1),
                                            material->Eval(direction, direction_to_next, normal, tex_coord))
                            * std::abs(normal.dot(direction_to_next))
                            / material->Pdf(direction, direction_to_next, normal)
                            / kRussianRoulette;
        }
    }

    return radiance_light + radiance_path;
}

global::Color Scene::Shade(const Intersection &intersection, int bounce, bool need_emission) const {
    Material *material = intersection.material;
    auto &position = intersection.position, &normal = intersection.normal, &direction = intersection.direction;
    auto &tex_coord = intersection.tex_coord;

    // light
    global::Color radiance_light = global::kBlack;
    if (material->HasEmitter() && need_emission) {
        radiance_light = material->emission();
    }

    if (bounce >= kMaxBounce) {
        return radiance_light;
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
        radiance_direct = global::Product(intersection_to_light.material->emission(),
                                          material->Eval(direction, direction_to_light, normal, tex_coord))
                          * normal.dot(direction_to_light)
                          * normal_light.dot(-direction_to_light)
                          / (position_light - position).squaredNorm()
                          / pdf_light;
    }

    // indirect light
    global::Color radiance_indirect = global::kBlack;
    if (RussianRoulette(bounce)) {
        // wi (inter to next)
        auto direction_to_next = material->Sample(direction, normal);
        if (direction_to_next == global::kNone) {
            return radiance_light + radiance_direct;
        }
        global::Vector position_new = position + normal * (normal.dot(direction_to_next) > 0 ? kEpsilon : -kEpsilon);
        // global::Vector position_new = position;
        Ray ray_to_next = Ray(position_new, direction_to_next);
        Intersection intersection_next;
        if (Intersect(&ray_to_next, &intersection_next)) {
            radiance_indirect = global::Product(Shade(intersection_next, bounce + 1, false),
                                                material->Eval(direction, direction_to_next, normal, tex_coord))
                                * std::abs(normal.dot(direction_to_next))
                                / material->Pdf(direction, direction_to_next, normal)
                                / kRussianRoulette;
        }
    }

    return radiance_light + radiance_direct + radiance_indirect;
}

bool Scene::RussianRoulette(int bounce) {
    return generator::Rand() < kRussianRoulette;
}

void Scene::SampleLight(Intersection *intersection, float *pdf) const {
    if (bvh_) {
        bvh_->SampleLight(intersection, pdf);
    } else {
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
}

global::Color Scene::ShadeMis(const Intersection &intersection, int bounce) const {
    Material *material = intersection.material;
    auto &position = intersection.position, &normal = intersection.normal, &direction = intersection.direction;
    auto &tex_coord = intersection.tex_coord;

    // light
    global::Color radiance_light = global::kBlack;
    if (material->HasEmitter()) {
        radiance_light = material->emission();
    }

    if (bounce >= kMaxBounce) {
        return radiance_light;
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
        float pdf_bsdf = material->Pdf(direction, direction_to_light, normal);
        float mis_wight = global::PowerHeuristic(pdf_bsdf, pdf_light);
        radiance_direct = global::Product(ShadeMis(intersection_to_light, bounce + 1),
                                          material->Eval(direction, direction_to_light, normal, tex_coord))
                          * normal.dot(direction_to_light)
                          * normal_light.dot(-direction_to_light)
                          / (position_light - position).squaredNorm()
                          / pdf_light
                          * mis_wight;
    }

    // indirect light
    global::Color radiance_indirect = global::kBlack;
    if (RussianRoulette(bounce)) {
        // wi (inter to next)
        auto direction_to_next = material->Sample(direction, normal);
        if (direction_to_next == global::kNone) {
            return radiance_light + radiance_direct;
        }
        global::Vector position_new = position + normal * (normal.dot(direction_to_next) > 0 ? kEpsilon : -kEpsilon);
        // global::Vector position_new = position;
        Ray ray_to_next = Ray(position_new, direction_to_next);
        Intersection intersection_next;
        if (Intersect(&ray_to_next, &intersection_next)) {
            float pdf_bsdf = material->Pdf(direction, direction_to_next, normal);
            ray_to_next = Ray(position_new, direction_to_next);
            pdf_light = 0.f;
            if (intersection_next.material->HasEmitter()) {
                pdf_light = 1.f / intersection_next.area;
            }
            float mis_wight = global::PowerHeuristic(pdf_bsdf, pdf_light);
            radiance_indirect = global::Product(ShadeMis(intersection_next, bounce + 1),
                                                material->Eval(direction, direction_to_next, normal, tex_coord))
                                * std::abs(normal.dot(direction_to_next))
                                / pdf_bsdf
                                * mis_wight
                                / kRussianRoulette;
        }
    }

    return radiance_light + radiance_direct + radiance_indirect;
}
