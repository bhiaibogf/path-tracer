//
// Created by bhiaibogf on 2022/4/13.
//

#include "skybox.h"

Skybox::Skybox(const std::string &path) {
    texture_ = new Texture(path);
}

global::Color Skybox::Sample(const global::Vector &direction) const {
    float phi = std::atan2(direction.x(), -direction.z()) * global::kInvTwoPi;
    float theta = std::acos(-direction.y()) * global::kInvPi;
    // 0.05 for day, -0.45 for dusk
    float offset = 0.f;
    if (texture_->path().find("day") != std::string::npos) {
        offset = 0.05f;
    }
    if (texture_->path().find("dusk") != std::string::npos) {
        offset = -0.45f;
    }
    return texture_->GetColor({phi + offset, theta});
}

std::ostream &operator<<(std::ostream &os, const Skybox &skybox) {
    os << "Skybox:\n  path: " << skybox.texture_->path();
    return os;
}
