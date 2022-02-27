//
// Created by bhiaibogf on 2022/2/23.
//

#include "refraction.h"

const float Refraction::n_s = 1e5f;

Refraction::Refraction(float n_i_) : ior_(n_i_) {}

global::Color
Refraction::Albedo(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const {
    global::Vector refract = global::Refract(wo, normal, ior_);
    if (wi == refract) {
        float schlick = global::Schlick(wo, normal, ior_);
        return {1 - schlick, 1 - schlick, 1 - schlick};
    }
    return global::kBlack;
}

global::Color Refraction::Eval(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const {
    return global::kBlack;
}

global::Vector Refraction::Sample(const global::Vector &wo, const global::Vector &normal, float *pdf) const {
    global::Vector refract = global::Refract(wo, normal, ior_);
    if (refract != global::kNone) {
        *pdf = global::kInf;
        return refract;
    }
    *pdf = 0.f;
    return global::kNone;
}

float Refraction::Pdf(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const {
    return 0.f;
}

std::ostream &operator<<(std::ostream &os, const Refraction &refraction) {
    os << "Refraction:\n\tni = " << refraction.ior_;
    return os;
}
