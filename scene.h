#include <memory>

class Agent;

class Scene {
public:

    Scene();
    ~Scene();

    int Load(const char*path);
    void AddAgent(const std::shared_ptr<Agent> &agent);


private:
};