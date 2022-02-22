//
// Created by bhiaibogf on 2022/2/22.
//

#include "lambert.h"

Lambert::Lambert(const global::Color &k_d) : albedo_(k_d) {}

bool Lambert::IsEmitter() const {
    return false;
}

global::Color Lambert::emission() const {
    return global::kBlack;
}

global::Color Lambert::Eval(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const {
    if (normal.dot(wi) > 0) {
        return albedo_ * global::k1PI;
    } else {
        return global::kBlack;
    }
}

global::Vector Lambert::Sample(const global::Vector &wo, const global::Vector &normal) const {
    // uniform sample on the hemisphere
    auto xi_1 = global::RandomFloat(), xi_2 = global::RandomFloat();
    float z = xi_1;
    float r = std::sqrt(1.0f - z * z), phi = global::kPi2 * xi_2;
    global::Vector local(r * std::cos(phi), r * std::sin(phi), z);
    return ToWorld(local, normal);
}

float Lambert::Pdf(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const {
    // uniform sample probability 1 / (2 * PI)
    if (normal.dot(wi) > 0) {
        return 1.f / global::kPi2;
    } else {
        return 0.f;
    }
}



