//
// Created by bhiaibogf on 2022/1/23.
//

#include "phong.h"

Phong::Phong(const global::Color &k_d, const global::Color &k_s, float n_s, float n_i)
        : k_d_(k_d), k_s_(k_s), n_s_(n_s), n_i_(n_i) {}

bool Phong::IsEmitter() const {
    return true;
}

global::Color Phong::emission() const {
    return global::kBlack;
}

global::Color Phong::Eval(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const {
    global::Color diffuse = k_d_ * global::k1PI;

    global::Color specular = global::kBlack;
    global::Vector reflect = global::Reflect(wi, normal);
    float alpha = normal.dot(reflect);
    if (alpha > 0) {
        specular = k_s_ * (n_s_ + 2) / global::kPi2 * std::pow(alpha, n_s_);
    }

    return diffuse + specular;
}

global::Vector Phong::Sample(const global::Vector &wo, const global::Vector &normal) const {
    // TODO no diffuse
    float cos_no = normal.dot(wo);
    if (cos_no > 0) {
        global::Vector reflect = global::Reflect(wo, normal);

        auto xi_1 = global::RandomFloat(), xi_2 = global::RandomFloat();

        float cos_theta = std::pow(xi_1, 1 / (n_s_ + 1));
        float sin_theta = std::sqrt(1 - cos_theta * cos_theta);

        float phi = global::kPi2 * xi_2;
        float sin_phi = std::sin(phi), cos_phi = std::cos(phi);

        return {cos_phi * sin_theta, sin_phi * sin_theta, cos_theta};
    }
    return wo;
}

float Phong::Pdf(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const {
    // TODO no diffuse
    global::Vector reflect = global::Reflect(wi, normal);
    float alpha = normal.dot(reflect);
    if (alpha > 0) {
        return (n_s_ + 1) / global::kPi2 * std::pow(alpha, n_s_);
    }
    return 0;
}
