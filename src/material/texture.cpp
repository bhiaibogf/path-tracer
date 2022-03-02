//
// Created by bhiaibogf on 2022/2/26.
//

#include "texture.h"

Texture::Texture(const std::string &filename) : path_(filename) {
    img_ = stbi_loadf(filename.c_str(), &width_, &height_, &channel_, 0);
}

global::Color Texture::GetColor(const global::TexCoord &tex_coord) const {
    float u = tex_coord.x(), v = tex_coord.y();

    int uu = int(std::floor(u)), vv = int(std::floor(v));
    u -= uu;
    v -= vv;

    uu %= 2;
    vv %= 2;
    if (uu == 1) u = 1 - u;
    if (vv == 1) v = 1 - v;

    int x = int(u * float(width_)), y = int(v * float(height_));
    int index = (x + y * width_) * channel_;
    global::Color color = global::Color(img_[index], img_[index + 1], img_[index + 2]);
    return color;
}
