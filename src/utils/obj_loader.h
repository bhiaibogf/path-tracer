//
// Created by bhiaibogf on 2022/2/22.
//

#ifndef PATH_TRACER_OBJ_LOADER_H
#define PATH_TRACER_OBJ_LOADER_H


#include "tinyobjloader/tiny_obj_loader.h"
#include "../material/phong.h"
#include "../material/light.h"
#include "../material/lambert.h"
#include "../geometry/object.h"

#include "global.h"

namespace obj_loader {
    std::vector<Object> Load(const std::string &model_path, const std::string &model_name);
}


#endif //PATH_TRACER_OBJ_LOADER_H
