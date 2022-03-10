//
// Created by bhiaibogf on 2022/2/26.
//

#ifndef PATH_TRACER_TEXTURE_H
#define PATH_TRACER_TEXTURE_H


#include "../utils/global.h"

#include "stb/stb_image.h"

class Texture {
public:
    explicit Texture(const std::string &path);

    ~Texture() = default;

    enum WrapMode {
        kRepeat,
        kMirror,
        kClamp
    };

    auto path() const { return "./" + path_; }

    global::Color GetColor(const global::TexCoord &tex_coord, WrapMode wrap_mode = kRepeat) const;

private:
    std::string path_;
    float *img_;
    int width_, height_, channel_;

};


#endif //PATH_TRACER_TEXTURE_H
