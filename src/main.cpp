#include "camera/camera.h"
#include "geometry/scene.h"
#include "renderer/path_tracer.h"
#include "utils/obj_loader.h"

int main() {
    Camera camera({278, 273, -800}, {278, 273, 0}, {0, 1, 0}, 40.f, 400, 300);

    std::string model_path = "scenes/", model_name = "cornell";
    Scene scene(obj_loader::Load(model_path, model_name));

    PathTracer renderer(camera, scene);
    renderer.Render(1);
    renderer.Save("test.exr");

    return 0;
}
