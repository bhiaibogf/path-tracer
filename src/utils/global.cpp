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

float global::Rand() {
    return uniform_distribution_(random_engine_);
}

float global::RandN() {
    return normal_distribution_(random_engine_);
}

global::Vector global::Reflect(const global::Vector &wi, const global::Vector &normal) {
    return 2 * normal.dot(wi) * normal - wi;
}

global::Vector global::Product(const global::Vector &a, const global::Vector &b) {
    return {a.x() * b.x(), a.y() * b.y(), a.z() * b.z()};
}
