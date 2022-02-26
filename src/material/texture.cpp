//
// Created by bhiaibogf on 2022/2/26.
//

#include "texture.h"

Texture::Texture(const std::string &filename) : path_(filename) {
    data_ = stbi_load(filename.c_str(), &width_, &height_, &channel_, 0);
}

global::Color Texture::GetColor(const global::TexCoord &tex_coord) const {
    float u = tex_coord.x(), v = tex_coord.y();
    u -= std::floor(u);
    v -= std::floor(v);

    int x = int(u * float(width_)), y = int(v * float(height_));
    int index = (x + y * width_) * channel_;
    global::Color color = global::Color(data_[index], data_[index + 1], data_[index + 2]);
    color /= 255.f;
    return color;
}
