//
// Created by bhiaibogf on 2022/4/13.
//

#ifndef PATH_TRACER_SKYBOX_H
#define PATH_TRACER_SKYBOX_H


#include "texture.h"
#include "../ray/ray.h"

class Skybox {
public:
    explicit Skybox(const std::string &path);

    ~Skybox() = default;

    global::Color Sample(const Ray &ray) const;

    friend std::ostream &operator<<(std::ostream &os, const Skybox &skybox);

private:
    Texture *texture_;

};


#endif //PATH_TRACER_SKYBOX_H
