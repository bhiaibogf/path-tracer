#include "camera/camera.h"
#include "scene/scene.h"
#include "renderer/path_tracer.h"
#include "utils/obj_loader.h"
#include "utils/timer.h"
#include "utils/xml_loader.h"

int main() {
    std::string model_path = "scenes/";

    // std::string model_name = "cornell";
    std::string model_name = "veach-mis";
    // std::string model_name = "cornell-box";
    // std::string model_name = "bedroom";

    int spp = 16;
    // int spp = 512;

    bool antialiasing = true;
    // bool antialiasing = false;

    bool use_bvh = true;
    // bool use_bvh = false;

    // bool sample_to_light = true;
    bool sample_to_light = false;

    bool mis = true;
    // bool mis = false;

    Timer timer;

    timer.StartTimer();
    std::cout << "Loading scene..." << std::endl;

    XmlLoader loader(model_path + model_name + "/" + model_name + ".xml");
    Camera *camera = loader.LoadCamera();
    auto *lights = new std::vector<global::Vector>();
    loader.LoadLights(lights);

    ObjLoader obj_loader(model_path, model_name);
    auto scene = new Scene();
    obj_loader.Load(*lights, scene);

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
    renderer.Render(spp, antialiasing, sample_to_light);

    timer.StopTimer();
    std::cout << "\nRender complete, using " << timer.GetTime() << " seconds." << std::endl;

    renderer.Save("img/" + model_name + "/"
                  + std::to_string(camera->width()) + "X" + std::to_string(camera->width())
                  + "-" + std::to_string(spp)
                  + (antialiasing ? "-antialiasing" : "")
                  + (use_bvh ? "-BVH" : "")
                  + (sample_to_light ? "-light" : "")
                  + (mis ? "-mis" : "")
                  + ".exr");

    return 0;
}
