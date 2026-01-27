#ifndef GAMEPLAY_H_DEFINED
#define GAMEPLAY_H_DEFINED

#define BORDER_MAX 80.0f
#define BORDER_MIN (-80.0f)
#define ASTEROID_COUNT 360

#include <DirectXMath.h>
#include <vector>

using namespace DirectX;

enum EntityType
{
    PLAYER,
    PROJECTILE,
    ASTEROID
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
#endif