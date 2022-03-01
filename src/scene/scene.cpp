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

global::Color Scene::Trace(Ray *ray, SampleType sample_type) const {
    Intersection intersection;
    if (Intersect(ray, &intersection)) {
        switch (sample_type) {
            case kSampleLight:
            case kSampleBsdf:
            case kMis:
            case kSampleBoth:
                return Shade(intersection, 0);
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

    // emission
    global::Color radiance_emission = global::kBlack;

    if (material->HasEmitter() && bounce == 0) {
        radiance_emission = material->emission();
    }

    if (bounce >= kMaxBounce) {
        return radiance_emission;
    }

    // direct light
    global::Color radiance_direct = global::kBlack;

    // sample from light
    float pdf_light;
    auto[direction_to_light, position_light] = SampleLight(position, &pdf_light);
    if (direction_to_light != global::kNone) {
        Intersection intersection_to_light;
        global::Vector position_to_light =
                position + normal * (normal.dot(direction_to_light) > 0 ? kEpsilon : -kEpsilon);
        Ray ray_to_light(position_to_light, direction_to_light);

        if (Intersect(&ray_to_light, &intersection_to_light)
            // check if the light is visible
            && (position_light - intersection_to_light.position).squaredNorm() < kEpsilon) {
            radiance_direct = global::Product(intersection_to_light.material->emission(),
                                              material->Eval(direction, direction_to_light, normal, tex_coord))
                              * std::abs(normal.dot(direction_to_light))
                              / pdf_light;
            // MIS
            float pdf_bsdf = material->Pdf(direction, direction_to_light, normal);
            float mis_wight = global::PowerHeuristic(pdf_light, pdf_bsdf);
            radiance_direct *= mis_wight;

            auto direction_another_light = material->Sample(direction, normal, &pdf_bsdf);
            auto position_another_light =
                    position + normal * (normal.dot(direction_another_light) > 0 ? kEpsilon : -kEpsilon);
            Ray ray_another_light = Ray(position_another_light, direction_another_light);
            Intersection intersection_another_light;

            if (Intersect(&ray_another_light, &intersection_another_light)
                && intersection_another_light.material->HasEmitter()) {
                pdf_light = PdfLight(position, intersection_another_light);
                mis_wight = global::PowerHeuristic(pdf_bsdf, pdf_light);
                radiance_direct +=
                        mis_wight
                        * global::Product(intersection_another_light.material->emission(),
                                          material->Eval(direction, direction_another_light, normal, tex_coord))
                        * std::abs(normal.dot(direction_another_light))
                        / pdf_bsdf;
            }
        }
    }

    if (!RussianRoulette(bounce)) {
        return radiance_emission + radiance_direct;
    }

    // indirect light
    global::Color radiance_indirect = global::kBlack;

    float pdf_bsdf;
    auto direction_next = material->Sample(direction, normal, &pdf_bsdf);
    if (direction_next == global::kNone) {
        return radiance_emission + radiance_direct;
    }

    auto position_next = position + normal * (normal.dot(direction_next) > 0 ? kEpsilon : -kEpsilon);
    Ray ray_next = Ray(position_next, direction_next);
    Intersection intersection_next;

    if (Intersect(&ray_next, &intersection_next)) {
        if (pdf_bsdf == global::kInf) {
            radiance_indirect
                    = global::Product(Shade(intersection_next, bounce + 1),
                                      ((Refraction *) material)->Albedo(direction, direction_next, normal))
                      / kRussianRoulette;
        } else {
            radiance_indirect
                    = global::Product(Shade(intersection_next, bounce + 1),
                                      material->Eval(direction, direction_next, normal, tex_coord))
                      * std::abs(normal.dot(direction_next))
                      / pdf_bsdf
                      / kRussianRoulette;
        }
    }

    return radiance_emission + radiance_direct + radiance_indirect;
}

bool Scene::RussianRoulette(int bounce) {
    return generator::Rand() < kRussianRoulette;
}

std::pair<global::Vector, global::Vector> Scene::SampleLight(const global::Vector &position, float *pdf) const {
    Intersection intersection_light;
    if (bvh_) {
        bvh_->SampleLight(&intersection_light, pdf);
    } else {
        float area_sum = 0;
        for (const auto &object: objects_) {
            area_sum += object->material()->emission().squaredNorm() * object->Area();
        }

        float random_area = generator::Rand() * area_sum;
        area_sum = 0;
        for (const auto &object: objects_) {
            area_sum += object->material()->emission().squaredNorm() * object->Area();
            if (random_area <= area_sum) {
                object->Sample(&intersection_light, pdf);
                break;
            }
        }
        (*pdf) /= area_sum;
    }
    auto direction_to_light = (intersection_light.position - position).normalized();
    if (intersection_light.normal.dot(-direction_to_light) < 0) {
        return std::make_pair(global::kNone, global::kNone);
    }
    *pdf = (*pdf) * (intersection_light.position - position).squaredNorm()
           / intersection_light.normal.dot(-direction_to_light);
    return {direction_to_light, intersection_light.position};
}

float Scene::PdfLight(const global::Vector &position, const Intersection &intersection_another_light) const {
    if (intersection_another_light.material->HasEmitter()) {
        return (intersection_another_light.position - position).squaredNorm()
               / std::abs(intersection_another_light.normal.dot(intersection_another_light.direction))
               * intersection_another_light.material->emission().squaredNorm()
               / bvh_->Area();
    }
    return 0.f;
}
