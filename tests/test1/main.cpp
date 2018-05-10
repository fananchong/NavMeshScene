#include <scene.h>
#include <agent.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>

long long get_tick_count(void)
{
    typedef std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> nanoClock_type;
    nanoClock_type tp = std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now());
    return tp.time_since_epoch().count();
}

class Player : public NavMeshScene::Agent
{
public:
    void OnHit(Agent* agent) override
    {
        changeDir();
    }

public:
    void changeDir()
    {
        float angle = float(rand() % 360);
        float vx = cos(3.14f * angle / 180);
        float vy = -sin(3.14f * angle / 180);
        float s = sqrt(vx*vx + vy*vy);
        vx = vx / s;
        vy = vy / s;
        float v[3] = { vx * 5, 0, vy * 5 };
        SetVelocity(v);
    }
};

int main(int argn, char *argv[]) {

    std::string path = "Meshes/scene1.obj.tile.bin";
    if (argn >= 2) {
        path = argv[1];
    }

    srand((unsigned int)(time(0)));

    NavMeshScene::StaticScene scene;
    if (int ec = scene.Load(path.c_str())) {
        std::cout << "load scene fail! errcode: " << ec << std::endl;
        return 1;
    }
    std::cout << "load scene success!" << std::endl;

    float* min = scene.GetBoundsMin();
    float* max = scene.GetBoundsMax();
    printf("width:%f, heigth:%f\n", max[0] - min[0], max[2] - min[2]);

    size_t PLAYER_COUNT = 5000;
    size_t TEST_COUNT = 10000;
    for (size_t i = 0; i < PLAYER_COUNT; i++)
    {
        auto agent = std::make_shared<Player>();
        scene.AddAgent(i + 1, agent);
        agent->RandomPosition();
        agent->changeDir();
    }
    printf("player count: %u\n", unsigned(PLAYER_COUNT));

    auto t1 = get_tick_count();
    for (size_t i = 0; i < TEST_COUNT; i++)
    {
        scene.Simulation(0.025f);
    }
    auto t2 = get_tick_count();
    printf("simulation cost:%12lldns %12.3fns/op %12.3fms/op\n", t2 - t1, float(t2 - t1) / TEST_COUNT, float(t2 - t1) / TEST_COUNT / 1000000);

    return 0;
}
