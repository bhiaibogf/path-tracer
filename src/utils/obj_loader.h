//
// Created by bhiaibogf on 2022/2/22.
//

#ifndef PATH_TRACER_OBJ_LOADER_H
#define PATH_TRACER_OBJ_LOADER_H


#include "tinyobjloader/tiny_obj_loader.h"
#include "../material/phong.h"
#include "../material/lambert.h"
#include "../material/refraction.h"
#include "../geometry/object.h"

#include "global.h"

class ObjLoader {
public:
    ObjLoader(const std::string &model_path, const std::string &model_name);

    ~ObjLoader() = default;

    std::vector<Object> Load();

private:
    std::string model_path_, filename_;
    tinyobj::ObjReader reader_;

    std::vector<Material *> materials_;
    std::vector<Object> objects_;

    void LoadMaterials();

    void LoadMeshes();

};


#endif //PATH_TRACER_OBJ_LOADER_H
