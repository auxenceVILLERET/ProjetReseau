#ifndef GAMEPLAY_H_DEFINED
#define GAMEPLAY_H_DEFINED

#define BORDER_MAX 50.0f
#define BORDER_MIN (-50.0f)

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

#endif