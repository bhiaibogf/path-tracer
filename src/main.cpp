#include "camera/camera.h"
#include "scene/scene.h"
#include "renderer/path_tracer.h"
#include "utils/obj_loader.h"
#include "utils/timer.h"
#include "utils/xml_loader.h"

#include <inih/INIReader.h>

int main() {
    INIReader reader("scene.ini");
    if (reader.ParseError() != 0) {
        std::cout << "Can't load 'scene.ini'\n";
        return 1;
    }

    std::string model_path = reader.Get("", "model_path", "scene") + "/";
    std::string model_name = reader.Get("", "model_name", "bedroom");

    int spp = int(reader.GetInteger("", "spp", 1));

    bool antialiasing = reader.GetBoolean("", "antialiasing", false);
    bool use_bvh = reader.GetBoolean("", "use_bvh", true);

    std::string shading_type_name = reader.Get("", "shading_type", "depth");
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

    Timer timer;

    timer.StartTimer();
    std::cout << "Loading scene..." << std::endl;

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

    timer.StopTimer();
    std::cout << "Load complete, using " << timer.GetTime() << " seconds." << std::endl;

    if (use_bvh) {
        timer.StartTimer();
        std::cout << "\nBuilding BVH..." << std::endl;

        scene->BuildBvh();

        timer.StopTimer();
        std::cout << "BVH built, using " << timer.GetTime() << " seconds." << std::endl;
    }

    timer.StartTimer();
    std::cout << "\nRendering..." << std::endl;

    PathTracer renderer(camera, scene);
    renderer.Render(spp, antialiasing, shading_type);

    timer.StopTimer();
    std::cout << "\nRender complete, using " << timer.GetTime() << " seconds." << std::endl;

    renderer.Save("img/" + model_name + "/"
                  + std::to_string(camera->width()) + "X" + std::to_string(camera->height())
                  + (spp == 1 ? "" : ("-" + std::to_string(spp)))
                  + (antialiasing ? "-antialiasing" : "")
                  + (use_bvh ? "-BVH" : "")
                  + "-" + shading_type_name
                  + ".exr");

    return 0;
}
