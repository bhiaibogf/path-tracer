//
// Created by bhiaibogf on 2022/4/13.
//

#include "skybox.h"

Skybox::Skybox(const std::string &path) {
    texture_ = new Texture(path);
}

global::Color Skybox::Sample(const Ray &ray) const {
    return texture_->GetColor({0, 0});
}

std::ostream &operator<<(std::ostream &os, const Skybox &skybox) {
    os << "Skybox:\n  path: " << skybox.texture_->path();
    return os;
}
