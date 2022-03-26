#include "camera/camera.h"
#include "scene/scene.h"
#include "renderer/path_tracer.h"
#include "utils/obj_loader.h"
#include "utils/timer.h"
#include "utils/xml_loader.h"

int main() {
    std::string model_path = "scenes/";

    // std::string model_name = "cornell-simple";
    // std::string model_name = "cornell-complex";
    // std::string model_name = "cornell-mirror";
    // std::string model_name = "cornell-sphere";

    // std::string model_name = "veach-mis";
    // std::string model_name = "cornell-box";
    std::string model_name = "bedroom";

    // std::string model_name = "staircase";

    int spp = 1;
    // int spp = 16;
    // int spp = 512;
    // int spp = 4096;

    // bool antialiasing = true;
    bool antialiasing = false;

    bool use_bvh = true;
    // bool use_bvh = false;

    // Scene::ShadingType shading_type = Scene::kUv;
    // Scene::ShadingType shading_type = Scene::kAlbedo;
    // Scene::ShadingType shading_type = Scene::kNormal;
    // Scene::ShadingType shading_type = Scene::kPosition;
    Scene::ShadingType shading_type = Scene::kDepth;

    // Scene::ShadingType shading_type = Scene::kSampleBsdf;
    // Scene::ShadingType shading_type = Scene::kSampleLight;
    // Scene::ShadingType shading_type = Scene::kMis;

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
                  + (shading_type == Scene::kUv ? "-uv" : "")
                  + (shading_type == Scene::kAlbedo ? "-albedo" : "")
                  + (shading_type == Scene::kPosition ? "-position" : "")
                  + (shading_type == Scene::kNormal ? "-normal" : "")
                  + (shading_type == Scene::kDepth ? "-depth" : "")
                  + (shading_type == Scene::kSampleBsdf ? "-bsdf" : "")
                  + (shading_type == Scene::kSampleLight ? "-light" : "")
                  + (shading_type == Scene::kMis ? "-mis" : "")
                  + ".exr");

    return 0;
}
