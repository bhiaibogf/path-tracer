//
// Created by bhiaibogf on 2022/2/21.
//

#include "light.h"

Light::Light(const global::Color &radiance) : radiance_(radiance) {
    // TODO calculate area
}

bool Light::IsEmitter() const {
    return true;
}

global::Color Light::emission() const {
    return radiance_;
}

global::Color Light::Eval(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const {
    assert(false);
}

global::Vector Light::Sample(const global::Vector &wo, const global::Vector &normal) const {
    assert(false);
}

float Light::Pdf(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const {
    assert(false);
}
