
class Agent {
public:

    Agent();
    ~Agent();

    inline int GetPosX() { return posX; }
    inline int GetPosY() { return posY; }
    inline int GetPosZ() { return posZ; }
    inline unsigned int GetRadius() { return radius; }

    inline void SetPosX(int v) { posX = v; }
    inline void SetPosY(int v) { posY = v; }
    inline void SetPosZ(int v) { posZ = v; }
    inline void SetRadius(int v) { radius = v; }

private:
    int posX;
    int posY;
    int posZ;
    unsigned int radius;
};