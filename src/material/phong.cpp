//
// Created by bhiaibogf on 2022/1/23.
//

#include "phong.h"

Phong::Phong(const global::Color &k_s, float n_s, float n_i)
        : k_s_(k_s), n_s_(n_s), n_i_(n_i) {}

global::Color Phong::Eval(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const {
    if (normal.dot(wo) > 0 && normal.dot(wi) > 0) {
        global::Vector reflect = global::Reflect(wi, normal);
        float alpha = wo.dot(reflect);
        if (alpha > 0) {
            return k_s_ * (n_s_ + 2) / global::kPi2 * std::pow(alpha, n_s_);
        }
    }
    return global::kBlack;
}

global::Vector Phong::Sample(const global::Vector &wo, const global::Vector &normal) const {
    float cos_no = normal.dot(wo);
    if (cos_no > 0) {
        global::Vector reflect = global::Reflect(wo, normal);

        auto xi_1 = global::Rand(), xi_2 = global::Rand();

        float z = std::pow(xi_1, 1 / (n_s_ + 1));
        float r = std::sqrt(1 - z * z);

        float phi = global::kPi2 * xi_2;
        float sin_phi = std::sin(phi), cos_phi = std::cos(phi);

        global::Vector local(r * cos_phi, r * sin_phi, z);
        return ToWorld(local, normal);
    }
    return -wo;
}

float Phong::Pdf(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const {
    if (normal.dot(wo) > 0 && normal.dot(wi) > 0) {
        global::Vector reflect = global::Reflect(wi, normal);
        float alpha = wo.dot(reflect);
        if (alpha > 0) {
            return (n_s_ + 1) / global::kPi2 * std::pow(alpha, n_s_);
        }
    }
    return 0;
}

std::ostream &operator<<(std::ostream &os, const Phong &phong) {
    using namespace global;
    os << "Phong:\n\tks = " << phong.k_s_ << "\n\tns = " << phong.n_s_ << "\n\tni = " << phong.n_i_;
    if (phong.HasEmitter()) {
        os << *((Material *) &phong);
    }
    return os;
}
