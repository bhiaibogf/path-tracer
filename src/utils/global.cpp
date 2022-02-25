//
// Created by bhiaibogf on 2022/2/18.
//

#include "global.h"

float global::Radius(float alpha) {
    return alpha / 180.f * kPi;
}

void global::UpdateProgress(float progress) {
    std::cout << "\r[";

    int bar_width = 70;
    int pos = bar_width * progress;
    for (int i = 0; i < bar_width; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }

    std::cout << "] " << int(progress * 100.0) << " %";
    std::cout.flush();
}

global::Vector global::Reflect(const global::Vector &wi, const global::Vector &normal) {
    return 2 * normal.dot(wi) * normal - wi;
}

global::Vector global::Product(const global::Vector &a, const global::Vector &b) {
    return {a.x() * b.x(), a.y() * b.y(), a.z() * b.z()};
}

global::Vector global::Refract(const global::Vector &wi, const global::Vector &normal, float ior) {
    float cos_ni = normal.dot(wi);
    // check which side of the surface we are on
    global::Vector normal_real;
    float eta;
    if (cos_ni > 0) {
        // we are on the outside of the surface, going in
        eta = 1.f / ior;
        normal_real = normal;
    } else {
        // we are inside the surface,
        cos_ni = -cos_ni;
        eta = ior;
        normal_real = -normal;
    }

    float sin_2_theta = (eta * eta * (1.0f - cos_ni * cos_ni));
    float cos_2_theta = 1.0f - sin_2_theta;
    if (cos_2_theta >= 0) {
        float cos_theta = std::sqrt(cos_2_theta);
        float nK = (eta * cos_ni) - cos_theta;
        return -wi * eta + normal_real * nK;
        // return 1 - fresnel_dielectric(cos_ni, eta);
    }
    return global::kNone;
}

float global::Schlick(const global::Vector &wi, const global::Vector &normal, float ior) {
    float cos_theta = normal.dot(wi);
    if (cos_theta < 0) {
        cos_theta = -cos_theta;
        ior = 1.f / ior;
    }
    float r_0 = (1.f - ior) / (1.f + ior);
    return r_0 + (1.f - r_0) * std::pow(1.f - cos_theta, 5.f);
}

std::ostream &global::operator<<(std::ostream &os, const Eigen::Vector3f &vector) {
    os << "[" << vector.x() << ", " << vector.y() << ", " << vector.z() << "]";
    return os;
}

std::ostream &global::operator<<(std::ostream &os, const Eigen::Vector2f &vector) {
    os << "[" << vector.x() << ", " << vector.y() << "]";
    return os;
}
