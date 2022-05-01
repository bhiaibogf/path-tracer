//
// Created by bhiaibogf on 2022/4/13.
//

#ifndef PATH_TRACER_SKYBOX_H
#define PATH_TRACER_SKYBOX_H


#include "texture.h"

class Skybox {
public:
    explicit Skybox(const std::string &path);

    ~Skybox() = default;

    global::Color Sample(const global::Vector &direction) const;

    friend std::ostream &operator<<(std::ostream &os, const Skybox &skybox);

private:
    Texture<global::Color> *texture_;

};


#endif //PATH_TRACER_SKYBOX_H
