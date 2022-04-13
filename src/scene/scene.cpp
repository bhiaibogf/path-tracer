//
// Created by bhiaibogf on 2022/1/23.
//

#include "scene.h"

const Eigen::Vector3f Scene::kBackgroundColor = global::Color(0.2f, 0.4f, 0.8f);
const float Scene::kEpsilonPosition = 1e-5f;
const float Scene::kEpsilonLight = 10.f * kEpsilonPosition;
const float Scene::kEpsilonPdf = 1e-6f;
const float Scene::kRussianRoulette = 0.9f;
const int Scene::kMaxBounce = 10;

Scene::Scene() {
    area_weighted_sum_ = 0.f;
    scale_ = 0.f;
    bound_ = Bound();

    bvh_ = nullptr;
    alias_table_ = nullptr;
}

void Scene::AddObject(Object *object) {
    objects_.push_back(object);
    bound_ |= object->primitive()->bound();
    if (object->material()->HasEmitter()) {
        area_weighted_sum_ += object->AreaWeighted();
    }
    scale_ = bound_.Diagonal().norm();
}

void Scene::BuildBvh() {
    bvh_ = new Bvh(objects_);
    alias_table_ = new AliasTable(objects_);
}

global::Color Scene::Trace(Ray *ray, ShadingType shading_type) const {
    Intersection intersection;
    if (Intersect(ray, &intersection)) {
        if (shading_type < kSampleLight) {
            return Shade(*ray, intersection, shading_type);
        } else {
            return Shade(intersection, 0, shading_type);
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
        for (const auto &object: objects_) {
            if (object->Intersect(ray, intersection)) {
                has_intersection = true;
            }
        }
        return has_intersection;
    }
}

global::Color Scene::Shade(const Ray &ray, const Intersection &intersection, Scene::ShadingType g_buffer_type) const {
    switch (g_buffer_type) {
        case kUv:
            return {intersection.tex_coord.x(), intersection.tex_coord.y(), 0.f};
        case kAlbedo:
            return intersection.material->Albedo(intersection.tex_coord);
        case kNormal:
            return intersection.normal;
        case kPosition:
            return intersection.position;
        case kDepth:
            return {ray.t(), ray.t(), ray.t()};
        default:
            return global::kBlack;
    }
}

global::Color Scene::Shade(const Intersection &intersection, int bounce, ShadingType sample_type) const {
    Material *material = intersection.material;

    // emission
    global::Color radiance_emission = global::kBlack;

    if (material->HasEmitter() && bounce == 0) {
        radiance_emission = material->emission();
    }

    if (bounce >= kMaxBounce) {
        return radiance_emission;
    }

    auto &position = intersection.position, &normal = intersection.normal, &direction = intersection.direction;
    auto &tex_coord = intersection.tex_coord;

    // direct light
    global::Color radiance_direct = global::kBlack;

    if (sample_type != kSampleBsdf) {
        // sample from light
        global::Color radiance_light = global::kBlack;

        float pdf_light;
        auto[direction_to_light, position_light] = SampleLight(position, &pdf_light);
        if (pdf_light > kEpsilonPdf) {
            global::Vector position_to_light =
                    position +
                    normal * scale_ * (normal.dot(direction_to_light) > 0 ? kEpsilonPosition : -kEpsilonPosition);
            Ray ray_to_light(position_to_light, direction_to_light);
            Intersection intersection_to_light;

            if (Intersect(&ray_to_light, &intersection_to_light)
                // check if the light is visible
                && (position_light - intersection_to_light.position).norm() < scale_ * kEpsilonLight) {
                radiance_light = global::Product(intersection_to_light.material->emission(),
                                                 material->Eval(direction, direction_to_light, normal, tex_coord))
                                 * normal.dot(direction_to_light)
                                 / pdf_light;
                if (sample_type == kMis) {
                    float pdf_bsdf = material->Pdf(direction, direction_to_light, normal);
                    float mis_wight = global::PowerHeuristic(pdf_light, pdf_bsdf);
                    radiance_light *= mis_wight;
                }
            }
        }

        radiance_direct += radiance_light;
    }

    if (sample_type != kSampleLight) {
        // sample from bsdf
        global::Color radiance_bsdf = global::kBlack;

        float pdf_bsdf;
        auto direction_another_light = material->Sample(direction, normal, &pdf_bsdf);
        if (pdf_bsdf > kEpsilonPdf) {
            auto position_another_light =
                    position +
                    normal * scale_ * (normal.dot(direction_another_light) > 0 ? kEpsilonPosition : -kEpsilonPosition);
            Ray ray_another_light = Ray(position_another_light, direction_another_light);
            Intersection intersection_another_light;

            if (Intersect(&ray_another_light, &intersection_another_light)
                && intersection_another_light.material->HasEmitter()
                && intersection_another_light.normal.dot(-direction_another_light) > 0) {
                if (pdf_bsdf == global::kInf) {
                    radiance_bsdf =
                            global::Product(intersection_another_light.material->emission(),
                                            ((Refraction *) material)->Albedo(direction, direction_another_light,
                                                                              normal));

                } else {
                    radiance_bsdf =
                            global::Product(intersection_another_light.material->emission(),
                                            material->Eval(direction, direction_another_light, normal, tex_coord))
                            * normal.dot(direction_another_light)
                            / pdf_bsdf;
                }
                if (sample_type == kMis) {
                    float pdf_light = PdfLight(position, intersection_another_light);
                    float mis_wight = global::PowerHeuristic(pdf_bsdf, pdf_light);
                    radiance_bsdf *= mis_wight;
                }
            }
        }

        radiance_direct += radiance_bsdf;
    }

    if (!RussianRoulette(bounce)) {
        return radiance_emission + radiance_direct;
    }

    // indirect light
    global::Color radiance_indirect = global::kBlack;

    float pdf_bsdf;
    auto direction_next = material->Sample(direction, normal, &pdf_bsdf);
    if (pdf_bsdf < kEpsilonPdf) {
        return radiance_emission + radiance_direct;
    }

    auto position_next =
            position + normal * scale_ * (normal.dot(direction_next) > 0 ? kEpsilonPosition : -kEpsilonPosition);
    Ray ray_next = Ray(position_next, direction_next);
    Intersection intersection_next;

    if (Intersect(&ray_next, &intersection_next)) {
        if (pdf_bsdf == global::kInf) {
            radiance_indirect
                    = global::Product(Shade(intersection_next, bounce + 1, sample_type),
                                      ((Refraction *) material)->Albedo(direction, direction_next, normal))
                      / kRussianRoulette;
        } else {
            radiance_indirect
                    = global::Product(Shade(intersection_next, bounce + 1, sample_type),
                                      material->Eval(direction, direction_next, normal, tex_coord))
                      * normal.dot(direction_next)
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
        // bvh_->SampleLight(&intersection_light, pdf);
        alias_table_->SampleLight(&intersection_light, pdf);
        if (*pdf == 0) {
            return {global::kNone, global::kNone};
        }
    } else {
        if (area_weighted_sum_ == 0) {
            *pdf = 0;
            return {global::kNone, global::kNone};
        }
        float random_area = generator::Rand() * area_weighted_sum_;
        float area_weighted_sum = 0;
        for (const auto &object: objects_) {
            if (object->material()->HasEmitter()) {
                area_weighted_sum += object->AreaWeighted();
                if (random_area <= area_weighted_sum) {
                    object->Sample(&intersection_light, pdf);
                    break;
                }
            }
        }
        (*pdf) /= area_weighted_sum_;
    }
    auto direction_light = (intersection_light.position - position).normalized();
    if (intersection_light.normal.dot(-direction_light) < 0) {
        *pdf = 0.f;
        return {global::kNone, global::kNone};
    }
    *pdf = (*pdf) * (intersection_light.position - position).squaredNorm()
           / intersection_light.normal.dot(-direction_light);
    return {direction_light, intersection_light.position};
}

float Scene::PdfLight(const global::Vector &position, const Intersection &intersection_another_light) const {
    if (intersection_another_light.material->HasEmitter()
        && intersection_another_light.normal.dot(intersection_another_light.direction) > 0) {
        return (intersection_another_light.position - position).squaredNorm()
               / intersection_another_light.normal.dot(intersection_another_light.direction)
               * intersection_another_light.weight
               / area_weighted_sum_;
    }
    return 0.f;
}
