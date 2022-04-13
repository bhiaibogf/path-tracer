#include "camera/camera.h"
#include "scene/scene.h"
#include "renderer/path_tracer.h"
#include "utils/obj_loader.h"
#include "utils/timer.h"
#include "utils/xml_loader.h"

#include <inih/INIReader.h>

int main() {
    Timer timer;

    INIReader reader("scene.ini");
    if (reader.ParseError() != 0) {
        std::cout << "Can't load scene.ini\n";
        return 1;
    } else {
        timer.StartTimer("Loading scene.ini");
    }

    std::string model_path = reader.Get("model", "model_path", "scene") + "/";
    std::string model_name = reader.Get("model", "model_name", "bedroom");

    int spp = int(reader.GetInteger("rendering", "spp", 1));

    bool antialiasing = reader.GetBoolean("rendering", "antialiasing", false);
    if (spp == 1) {
        antialiasing = false;
    }
    bool use_bvh = reader.GetBoolean("rendering", "use_bvh", true);

    std::string shading_type_name = reader.Get("rendering", "shading_type", "depth");
    Scene::ShadingType shading_type = Scene::kDepth;
    if (shading_type_name == "uv") {
        shading_type = Scene::kUv;
    } else if (shading_type_name == "albedo") {
        shading_type = Scene::kAlbedo;
    } else if (shading_type_name == "normal") {
        shading_type = Scene::kNormal;
    } else if (shading_type_name == "position") {
        shading_type = Scene::kPosition;
    } else if (shading_type_name == "depth") {
        shading_type = Scene::kDepth;
    } else if (shading_type_name == "light") {
        shading_type = Scene::kSampleLight;
    } else if (shading_type_name == "bsdf") {
        shading_type = Scene::kSampleBsdf;
    } else if (shading_type_name == "mis") {
        shading_type = Scene::kMis;
    }
    timer.StopTimer("scene.ini loaded");

    timer.StartTimer("\nLoading scene");

    XmlLoader loader(model_path + model_name + "/" + model_name + ".xml");
    Camera *camera = loader.LoadCamera();
    std::cout << *camera << std::endl;

    std::map<std::string, global::Vector> lights;
    loader.LoadLights(&lights);

    std::vector<Sphere *> spheres;
    loader.LoadSphere(&spheres);

    ObjLoader obj_loader(model_path, model_name);
    auto scene = new Scene();
    obj_loader.Load(lights, spheres, scene);

    timer.StopTimer("Scene Loaded");

    if (use_bvh) {
        timer.StartTimer("\nBuilding BVH");
        scene->BuildBvh();
        timer.StopTimer("BVH built");
    }

    timer.StartTimer("\nRendering");

    PathTracer renderer(camera, scene);
    renderer.Render(spp, antialiasing, shading_type);

    timer.StopTimer("\nRendering finished");

    renderer.Save("img/" + model_name + "/"
                  + std::to_string(camera->width()) + "X" + std::to_string(camera->height())
                  + (spp == 1 ? "" : ("-" + std::to_string(spp)))
                  + (antialiasing ? "-antialiasing" : "")
                  + (use_bvh ? "-BVH" : "")
                  + "-" + shading_type_name
                  + ".exr");

    return 0;
}
