#ifndef GAMEPLAY_H_DEFINED
#define GAMEPLAY_H_DEFINED

#define BORDER_MAX 80.0f
#define BORDER_MIN (-80.0f)
#define ASTEROID_COUNT 360

#include <DirectXMath.h>
#include <vector>

using namespace DirectX;

inline const XMFLOAT3 ARENA_CENTER = { 0.0f, 0.0f, 0.0f };

enum EntityType
{
    PLAYER,
    PROJECTILE,
    ASTEROID,
	POWERUP
};

inline std::vector<XMFLOAT3> SPAWN_POINTS =
    {
        {BORDER_MAX - 5.0f, BORDER_MAX - 5.0f, BORDER_MAX - 5.0f},
        {BORDER_MIN + 5.0f, BORDER_MAX - 5.0f, BORDER_MAX - 5.0f},
        {BORDER_MAX - 5.0f, BORDER_MIN + 5.0f, BORDER_MAX - 5.0f},
        {BORDER_MAX - 5.0f, BORDER_MAX - 5.0f, BORDER_MIN + 5.0f},
        {BORDER_MIN + 5.0f, BORDER_MIN + 5.0f, BORDER_MAX - 5.0f},
        {BORDER_MIN + 5.0f, BORDER_MAX - 5.0f, BORDER_MIN + 5.0f},
        {BORDER_MAX - 5.0f, BORDER_MIN + 5.0f, BORDER_MIN + 5.0f},
        {BORDER_MIN + 5.0f, BORDER_MIN + 5.0f, BORDER_MIN + 5.0f},
    };

inline XMFLOAT3 GetSpawnPoint()
{
    int i = rand() % SPAWN_POINTS.size();
    return SPAWN_POINTS[i];
}


inline float RandomRange(float min, float max)
{
    return min + static_cast<float>(rand()) / RAND_MAX * (max - min);
}

inline bool WorldToScreen(XMFLOAT3 const& world, XMFLOAT2& screen, XMFLOAT4X4 viewProj,int screenWidth, int screenHeight)
{

    float clip[4];

    clip[0] = world.x * viewProj.m[0][0] + world.y * viewProj.m[1][0] + world.z * viewProj.m[2][0] + viewProj.m[3][0];
    clip[1] = world.x * viewProj.m[0][1] + world.y * viewProj.m[1][1] + world.z * viewProj.m[2][1] + viewProj.m[3][1];
    clip[2] = world.x * viewProj.m[0][2] + world.y * viewProj.m[1][2] + world.z * viewProj.m[2][2] + viewProj.m[3][2];
    clip[3] = world.x * viewProj.m[0][3] + world.y * viewProj.m[1][3] + world.z * viewProj.m[2][3] + viewProj.m[3][3];

    if (clip[3] <= 0.0f)
        return false;

    float ndcX = clip[0] / clip[3];
    float ndcY = clip[1] / clip[3];

    screen.x = (ndcX * 0.5f + 0.5f) * static_cast<float>(screenWidth);
    screen.y = (1.0f - (ndcY * 0.5f + 0.5f)) * static_cast<float>(screenHeight);

    return true;
}
#endif