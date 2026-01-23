#ifndef MANAGERMETHODS_H_DEFINED
#define MANAGERMETHODS_H_DEFINED
#include <DirectXMath.h>
#include <vector>

using namespace DirectX;

struct ClientInfo;

class ManagerMethods
{
public:
    static void Init();
    static void SendCreationPackets(ClientInfo* pTarget);

    static void HandleDirtyEntities();

    static void InitMap();
};

#endif
