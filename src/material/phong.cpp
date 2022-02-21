//
// Created by bhiaibogf on 2022/1/23.
//

#include "phong.h"

bool Phong::IsEmitter() const {
    return true;
}

global::Color Phong::emission() const {
    return global::kBlack;
}

global::Color Phong::Eval(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const {
    // TODO
}

global::Vector Phong::Sample(const global::Vector &wo, const global::Vector &normal) const {
    // TODO
}

float Phong::Pdf(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const {
    // TODO
    return 0;
}
