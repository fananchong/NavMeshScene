#include <scene.h>
#include <iostream>
#include <thread>

int main(int argn, char *argv[]) {

    if (argn < 2) {
        std::cout << "usage: example1 [path]" << std::endl;
        return 1;
    }

    NavMeshScene::Scene scene;
    if (int ec = scene.Load(argv[1])) {
        std::cout << "load scene fail! errcode: " << ec << std::endl;
        return 1;
    }
    std::cout << "load scene success!" << std::endl;

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}