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

typedef NavMeshScene::StaticScene SceneType1;
typedef NavMeshScene::DynamicScene SceneType2;

template<class T>
int test(T& scene, const std::string& path)
{
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

int main(int argn, char *argv[]) {

    std::string path1 = "Meshes/scene1.obj.tile.bin";
    std::string path2 = "Meshes/scene1.obj.tilecache.bin";

    srand((unsigned int)(time(0)));

    printf("Static Scene ==============================================\n");
    SceneType1 scn1;
    test<SceneType1>(scn1, path1);
    printf("\n\n");

    printf("Dynamic Scene (Type 1) ==============================================\n");
    SceneType2 scn2(NavMeshScene::DynamicScene::HEIGHT_MODE_1);
    test<SceneType2>(scn2, path1);
    printf("\n\n");

    printf("Dynamic Scene (Type 2) ==============================================\n");
    SceneType2 scn3(NavMeshScene::DynamicScene::HEIGHT_MODE_2);
    test<SceneType2>(scn3, path2);
    printf("\n\n");
    return 0;
}
