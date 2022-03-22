//
// Created by bhiaibogf on 2022/2/23.
//

#include "refraction.h"

Refraction::Refraction(float n_i_) : ior_(n_i_) {}

global::Color
Refraction::Albedo(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const {
    float schlick = global::Schlick(wo, normal, ior_);

    global::Vector refract = global::Refract(wo, normal, ior_);
    if (refract != global::kNone) {
        if (refract == wi) {
            float eta = normal.dot(wo) > 0 ? 1.f / ior_ : ior_;
            float eta_2 = eta * eta;
            return {eta_2, eta_2, eta_2};
        } else {
            return {1.f, 1.f, 1.f};
        }
    } else {
        return {schlick, schlick, schlick};
    }
}

global::Color Refraction::Eval(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const {
    return global::kBlack;
}

global::Vector Refraction::Sample(const global::Vector &wo, const global::Vector &normal, float *pdf) const {
    *pdf = global::kInf;

    float schlick = global::Schlick(wo, normal, ior_);
    global::Vector refract = global::Refract(wo, normal, ior_);
    global::Vector reflect = global::Reflect(wo, normal);
    if (refract != global::kNone) {
        if (generator::Rand() < schlick) {
            return reflect;
        } else {
            return refract;
        }
    } else {
        return reflect;
    }
}

float Refraction::Pdf(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const {
    return 0.f;
}

std::ostream &operator<<(std::ostream &os, const Refraction &refraction) {
    os << "Refraction:\n\tni = " << refraction.ior_;
    return os;
}
