#include <iostream>
#include <time.h>
#include "helper.h"

std::shared_ptr<Mesh> gMesh;

void OnUpdate(float delta) {

}

void OnRender() {
    float cellSize = 0.3f;
    DrawMode drawmode = DRAWMODE_NAVMESH;
    RenderMesh(gMesh, cellSize, drawmode);
}

void OnRenderOverlay(double* proj, double* model, int* view) {

}

int main(int argn, char *argv[]) {

    if (argn < 1) {
        std::cout << "usage: example2 [netmesh path] [obj path]" << std::endl;
        return 1;
    }

    srand(unsigned int(time(0)));

    gMesh = LoadMesh(argv[1]);
    if (!gMesh) {
        return 1;
    }

    InitWindow(OnUpdate, OnRender, OnRenderOverlay);

    return 0;
}