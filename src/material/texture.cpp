//
// Created by bhiaibogf on 2022/2/26.
//

#include "texture.h"

template
class Texture<float>;

template
class Texture<global::Color>;

template<class T>
Texture<T>::Texture(const std::string &path):path_(path) {
    stbi_set_flip_vertically_on_load(true);
    img_ = stbi_loadf(path.c_str(), &width_, &height_, &channel_, 0);
}

template<class T>
T Texture<T>::GetColor(const global::TexCoord &tex_coord, Texture::WrapMode wrap_mode) const {
    float u = tex_coord.x(), v = tex_coord.y();

    switch (wrap_mode) {
        case kClamp:
            u = std::clamp(u, 0.f, 1.f);
            v = std::clamp(v, 0.f, 1.f);
            break;
        case kRepeat:
            u = u - std::floor(u);
            v = v - std::floor(v);
            break;
        case kMirror:
            int u_int = int(std::floor(u)), v_int = int(std::floor(v));

            u -= u_int;
            v -= v_int;

            if (u_int % 2 == 1) {
                u = 1 - u;
            }
            if (v_int % 2 == 1) {
                v = 1 - v;
            }
            break;
    }

    int x = int(u * float(width_ - 1)), y = int(v * float(height_ - 1));
    int index = (x + y * width_) * channel_;
    if constexpr (std::is_same<T, float>::value) {
        return img_[index];
    } else if constexpr (std::is_same<T, global::Color>::value) {
        return {img_[index], img_[index + 1], img_[index + 2]};
    } else {
        std::cout << "Wrong type" << std::endl;
    }
}
