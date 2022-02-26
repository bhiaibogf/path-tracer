//
// Created by bhiaibogf on 2022/2/26.
//

#ifndef PATH_TRACER_TEXTURE_H
#define PATH_TRACER_TEXTURE_H


#include "../utils/global.h"

#include "stb/stb_image.h"

class Texture {
public:
    explicit Texture(const std::string &filename);

    ~Texture() = default;

    auto path() const { return "./" + path_; }

    global::Color GetColor(const global::TexCoord &tex_coord) const;

private:
    std::string path_;
    unsigned char *data_;
    int width_, height_, channel_;

};


#endif //PATH_TRACER_TEXTURE_H