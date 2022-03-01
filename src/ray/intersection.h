//
// Created by bhiaibogf on 2022/2/21.
//

#ifndef PATH_TRACER_INTERSECTION_H
#define PATH_TRACER_INTERSECTION_H


#include "../material/material.h"

struct Intersection {
    Material *material;
    global::Vector position, normal, direction;
    global::TexCoord tex_coord;
};


#endif //PATH_TRACER_INTERSECTION_H
