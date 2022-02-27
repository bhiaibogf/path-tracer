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
                return Shade(intersection, 0, sample_type);
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

// Whitted Style
global::Color Scene::Shade(const Intersection &intersection, int bounce) const {
    Material *material = intersection.material;
    auto &position = intersection.position, &normal = intersection.normal, &direction = intersection.direction;
    auto &tex_coord = intersection.tex_coord;

    // light
    global::Color radiance_light = global::kBlack;
    if (material->HasEmitter() && bounce == 0) {
        radiance_light = material->emission();
    }

    if (bounce >= kMaxBounce) {
        return radiance_light;
    }

    // direct light
    global::Color radiance_direct = global::kBlack;

    // sample from light
    float pdf_light;
    auto direction_to_light = SampleLight(position, &pdf_light);

    global::Vector position_new = position + normal * (normal.dot(direction_to_light) > 0 ? kEpsilon : -kEpsilon);

    // check if the light is visible
    Intersection intersection_to_light;
    Ray ray_to_light(position_new, direction_to_light);
    if (Intersect(&ray_to_light, &intersection_to_light)) {
        radiance_direct = global::Product(intersection_to_light.material->emission(),
                                          material->Eval(direction, direction_to_light, normal, tex_coord))
                          * std::abs(normal.dot(direction_to_light))
                          / pdf_light;
    }

    // indirect light
    global::Color radiance_indirect = global::kBlack;
    if (RussianRoulette(bounce)) {
        // wi (inter to next)
        float pdf_bsdf;
        auto direction_to_next = material->Sample(direction, normal, &pdf_bsdf);
        if (direction_to_next != global::kNone) {
            global::Vector position_new =
                    position + normal * (normal.dot(direction_to_next) > 0 ? kEpsilon : -kEpsilon);
            Ray ray_to_next = Ray(position_new, direction_to_next);
            Intersection intersection_next;
            if (Intersect(&ray_to_next, &intersection_next)) {
                if (pdf_bsdf == global::kInf) {
                    radiance_indirect
                            = global::Product(Shade(intersection_next, bounce + 1),
                                              ((Refraction *) material)->Albedo(direction, direction_to_next, normal))
                              / kRussianRoulette;
                } else {
                    radiance_indirect
                            = global::Product(Shade(intersection_next, bounce + 1),
                                              material->Eval(direction, direction_to_next, normal, tex_coord))
                              * std::abs(normal.dot(direction_to_next))
                              / pdf_bsdf
                              / kRussianRoulette;
                }
            }
        }
    }

    return radiance_light + radiance_direct + radiance_indirect;
}

bool Scene::RussianRoulette(int bounce) {
    return generator::Rand() < kRussianRoulette;
}

global::Vector Scene::SampleLight(const global::Vector &position, float *pdf) const {
    Intersection intersection_light;
    if (bvh_) {
        bvh_->SampleLight(&intersection_light, pdf);
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
                    object->Sample(&intersection_light, pdf);
                    break;
                }
            }
        }
    }
    auto direction_to_light = (intersection_light.position - position).normalized();
    *pdf = (*pdf) * (intersection_light.position - position).squaredNorm()
           / std::abs(intersection_light.normal.dot(-direction_to_light));
    return direction_to_light;
}

float Scene::PdfLight(const global::Vector &position, const Intersection &intersection_next) {
    if (intersection_next.material->HasEmitter()) {
        return (intersection_next.position - position).squaredNorm()
               / intersection_next.area
               / std::abs(intersection_next.normal.dot(intersection_next.direction));
    }
    return 0.f;
}

global::Color Scene::Shade(const Intersection &intersection, int bounce, SampleType sample_type) const {
    Material *material = intersection.material;
    auto &position = intersection.position, &normal = intersection.normal, &direction = intersection.direction;
    auto &tex_coord = intersection.tex_coord;

    // emission
    global::Color radiance_emission = global::kBlack;

    if (material->HasEmitter()) {
        radiance_emission = material->emission();
    }

    if (bounce >= kMaxBounce || !RussianRoulette(bounce)) {
        return radiance_emission;
    }

    // sample light
    global::Color radiance_light = global::kBlack;

    if (sample_type != kSampleBsdf) {
        float pdf_light;
        auto direction_to_light = SampleLight(position, &pdf_light);

        global::Vector position_new = position + normal * (normal.dot(direction_to_light) > 0 ? kEpsilon : -kEpsilon);
        Ray ray_to_light(position_new, direction_to_light);
        Intersection intersection_to_light;

        if (Intersect(&ray_to_light, &intersection_to_light)) {
            radiance_light = global::Product(Shade(intersection_to_light, bounce + 1, sample_type),
                                             material->Eval(direction, direction_to_light, normal, tex_coord))
                             * std::abs(normal.dot(direction_to_light))
                             / pdf_light
                             / kRussianRoulette;
            if (sample_type == kSampleLight) {
                return radiance_emission + radiance_light;
            }
            float pdf_bsdf = material->Pdf(direction, direction_to_light, normal);
            float mis_wight = global::PowerHeuristic(pdf_light, pdf_bsdf);
            radiance_light *= mis_wight;
        }
    }

    // sample bsdf
    global::Color radiance_bsdf = global::kBlack;

    float pdf_bsdf;
    auto direction_to_next = material->Sample(direction, normal, &pdf_bsdf);
    if (direction_to_next == global::kNone) {
        return radiance_emission + radiance_light;
    }

    global::Vector position_new = position + normal * (normal.dot(direction_to_next) > 0 ? kEpsilon : -kEpsilon);
    Ray ray_to_next = Ray(position_new, direction_to_next);
    Intersection intersection_next;

    if (Intersect(&ray_to_next, &intersection_next)) {
        if (pdf_bsdf == global::kInf) {
            radiance_bsdf = global::Product(Shade(intersection_next, bounce + 1, sample_type),
                                            ((Refraction *) material)->Albedo(direction, direction_to_next, normal))
                            / kRussianRoulette;
        } else {
            radiance_bsdf = global::Product(Shade(intersection_next, bounce + 1, sample_type),
                                            material->Eval(direction, direction_to_next, normal, tex_coord))
                            * std::abs(normal.dot(direction_to_next))
                            / pdf_bsdf
                            / kRussianRoulette;
        }
        if (sample_type == kSampleBsdf) {
            return radiance_emission + radiance_bsdf;
        }
        float pdf_light = PdfLight(position, intersection_next);
        float mis_wight = global::PowerHeuristic(pdf_bsdf, pdf_light);
        radiance_bsdf *= mis_wight;
    }

    return radiance_emission + radiance_light + radiance_bsdf;
}
