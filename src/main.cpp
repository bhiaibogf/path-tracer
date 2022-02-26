#include "camera/camera.h"
#include "scene/scene.h"
#include "renderer/path_tracer.h"
#include "utils/obj_loader.h"
#include "utils/timer.h"
#include "utils/xml_loader.h"

int main() {
    Timer timer;

    timer.StartTimer();
    std::cout << "Loading scene..." << std::endl;

    std::string model_path = "scenes/", model_name = "cornell-box";

    XmlLoader loader(model_path + model_name + "/" + model_name + ".xml");
    Camera *camera = loader.LoadCamera();
    auto *lights = new std::vector<global::Vector>();
    loader.LoadLights(lights);

    ObjLoader obj_loader(model_path, model_name);
    auto scene = new Scene();
    obj_loader.Load(*lights, scene);

    timer.StopTimer();
    std::cout << "Load complete, using " << timer.GetTime() << " seconds." << std::endl;

    timer.StartTimer();
    std::cout << "\nBuilding BVH..." << std::endl;

    scene->BuildBvh();

    timer.StopTimer();
    std::cout << "BVH built, using " << timer.GetTime() << " seconds." << std::endl;

    timer.StartTimer();
    std::cout << "\nRendering..." << std::endl;

    PathTracer renderer(camera, scene);
    int spp = 16;
    bool antialiasing = true;
    renderer.Render(spp, antialiasing);

    timer.StopTimer();
    std::cout << "\nRender complete, using " << timer.GetTime() << " seconds." << std::endl;

    renderer.Save(model_name + "-" + std::to_string(spp) + "spp" + (antialiasing ? "-antialiasing" : "") + ".exr");

    return 0;
}
