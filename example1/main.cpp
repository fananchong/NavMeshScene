#include <scene.h>
#include <agent.h>
#include <iostream>
#include <thread>

int main(int argn, char *argv[]) {

    if (argn < 2) {
        std::cout << "usage: example1 [path]" << std::endl;
        return 1;
    }

    srand(unsigned int(time(0)));

    NavMeshScene::Scene scene;
    if (int ec = scene.Load(argv[1])) {
        std::cout << "load scene fail! errcode: " << ec << std::endl;
        return 1;
    }
    std::cout << "load scene success!" << std::endl;

    auto agent = std::make_shared< NavMeshScene::Agent>();
    scene.AddAgent(1, agent);
    agent->RandomPosition();

    auto pos = agent->GetPosition();
    std::cout << "x=" << pos[0] << ", y=" << pos[1] << ", z=" << pos[2] << std::endl;

    float velocity[3] = { 5,0,5 };
    agent->SetVelocity(velocity);

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(25));
        scene.Simulation(0.025f);
        auto pos = agent->GetPosition();
        std::cout << "x=" << pos[0] << ", y=" << pos[1] << ", z=" << pos[2] << std::endl;
    }

    return 0;
}