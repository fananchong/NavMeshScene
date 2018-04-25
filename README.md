# NavMeshScene


## 使用示例

```c++
int main(int argn, char *argv[]) {
    srand(unsigned int(time(0)));
    std::string path = "Meshes/nav_test.obj.tile.bin";
    NavMeshScene::StaticScene scene;
    if (int ec = scene.Load(path.c_str())) {
        std::cout << "load scene fail! errcode: " << ec << std::endl;
        return 1;
    }
    std::cout << "load scene success!" << std::endl;

    auto agent = std::make_shared<NavMeshScene::Agent>();
    scene.AddAgent(1, agent);
    agent->RandomPosition();

    auto pos = agent->GetPosition();

    float velocity[3] = { 5,0,5 };
    agent->SetVelocity(velocity);

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(25));
        scene.Simulation(0.025f);
    }

    return 0;
}
```
