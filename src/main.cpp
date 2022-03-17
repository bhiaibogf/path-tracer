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
    std::string model_name = "cornell-sphere";
    // std::string model_name = "veach-mis";
    // std::string model_name = "cornell-box";
    // std::string model_name = "bedroom";

    // int spp = 16;
    int spp = 512;

    bool antialiasing = true;
    // bool antialiasing = false;

    bool use_bvh = true;
    // bool use_bvh = false;

    // Scene::SampleType sample_type = Scene::kSampleBoth;
    Scene::SampleType sample_type = Scene::kSampleBsdf;
    // Scene::SampleType sample_type = Scene::kSampleLight;
    // Scene::SampleType sample_type = Scene::kMis;

    Timer timer;

    timer.StartTimer();
    std::cout << "Loading scene..." << std::endl;

    XmlLoader loader(model_path + model_name + "/" + model_name + ".xml");
    Camera *camera = loader.LoadCamera();

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
    renderer.Render(spp, antialiasing, sample_type);

    timer.StopTimer();
    std::cout << "\nRender complete, using " << timer.GetTime() << " seconds." << std::endl;

    renderer.Save("img/" + model_name + "/"
                  + std::to_string(camera->width()) + "X" + std::to_string(camera->height())
                  + "-" + std::to_string(spp)
                  + (antialiasing ? "-antialiasing" : "")
                  + (use_bvh ? "-BVH" : "")
                  + (sample_type == Scene::kSampleBsdf ? "-bsdf" : "")
                  + (sample_type == Scene::kSampleLight ? "-light" : "")
                  + (sample_type == Scene::kSampleBoth ? "-both" : "")
                  + (sample_type == Scene::kMis ? "-mis" : "")
                  + ".exr");

    return 0;
}
