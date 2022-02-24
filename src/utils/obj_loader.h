//
// Created by bhiaibogf on 2022/2/22.
//

#ifndef PATH_TRACER_OBJ_LOADER_H
#define PATH_TRACER_OBJ_LOADER_H


#include "../material/phong.h"
#include "../material/lambert.h"
#include "../material/refraction.h"
#include "../scene/scene.h"

#include "tinyobjloader/tiny_obj_loader.h"

class ObjLoader {
public:
    ObjLoader(const std::string &model_path, const std::string &model_name);

    ~ObjLoader() = default;

    void Load(Scene *scene);

private:
    std::string model_path_, filename_;
    tinyobj::ObjReader reader_;

    std::vector<Material *> materials_;

    void LoadMaterials();

    void LoadMeshes(Scene *scene);

};


#endif //PATH_TRACER_OBJ_LOADER_H
