//
// Created by bhiaibogf on 2022/2/18.
//

#include "global.h"

float global::Radius(float alpha) {
    return alpha / 180.f * kPi;
}

float global::Degree(float alpha) {
    return alpha * 180.f * kInvPi;
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
    return (a.array() * b.array()).matrix();
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
    }
    return global::kNone;
}

float global::Schlick(const global::Vector &wi, const global::Vector &normal, float ior) {
    float cos_theta = normal.dot(wi);
    float f_0 = (1.f - ior) / (1.f + ior);
    f_0 *= f_0;
    return f_0 + (1.f - f_0) * std::pow(1.f - std::abs(cos_theta), 5.f);
}

std::ostream &global::operator<<(std::ostream &os, const Eigen::Vector3f &vector) {
    os << "[" << vector.x() << ", " << vector.y() << ", " << vector.z() << "]";
    return os;
}

std::ostream &global::operator<<(std::ostream &os, const Eigen::Vector2f &vector) {
    os << "[" << vector.x() << ", " << vector.y() << "]";
    return os;
}

float global::PowerHeuristic(float pdf_sample, float pdf_other, float beta) {
    // NOTE: inf is ok!
    assert(pdf_sample >= 0);
    assert(pdf_other >= 0);

    float r, mis;
    if (pdf_sample > pdf_other) {
        r = pdf_other / pdf_sample;
        mis = 1 / (1 + r * r);
    } else if (pdf_sample < pdf_other) {
        r = pdf_sample / pdf_other;
        mis = 1 - 1 / (1 + r * r);
    } else {
        // avoid (possible, but extremely rare) inf/inf cases
        assert(pdf_sample == pdf_other);
        r = 1.0f;
        mis = 0.5f;
    }
    assert(r >= 0);
    assert(r <= 1);
    assert(mis >= 0);
    assert(mis <= 1);
    return mis;
}

float global::Luminance(const global::Color &color) {
    return 0.212671f * color.x() + 0.715160f * color.y() + 0.072169f * color.z();
}
