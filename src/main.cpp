#include "camera/camera.h"
#include "geometry/scene.h"
#include "renderer/path_tracer.h"

int main() {
    Camera camera({0, 1, 6.8}, {0, 1, 5.8}, {0, 1, 0}, 19.5f, 256, 256);
    Scene scene;

    PathTracer renderer(camera, scene);
    renderer.Render(1);
    renderer.Save("test.exr");

    return 0;
}
