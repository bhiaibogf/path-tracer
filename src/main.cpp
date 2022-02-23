#include "camera/camera.h"
#include "geometry/scene.h"
#include "renderer/path_tracer.h"
#include "utils/obj_loader.h"
#include "utils/timer.h"

int main() {
    Timer timer;

    timer.StartTimer();
    std::cout << "Loading scene..." << std::endl;

    Camera camera({278, 273, -800}, {278, 273, 0}, {0, 1, 0}, 40.f, 256, 256);

    std::string model_path = "scenes/", model_name = "cornell";
    ObjLoader obj_loader(model_path, model_name);
    Scene scene(obj_loader.Load());

    timer.StopTimer();
    std::cout << "Load complete, using " << timer.GetTime() << " seconds." << std::endl;

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
