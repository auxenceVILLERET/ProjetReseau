#ifndef MANAGERMETHODS_H_DEFINED
#define MANAGERMETHODS_H_DEFINED
#include <DirectXMath.h>
#include <vector>

#include "Packet.h"

using namespace DirectX;

class ManagerMethods
{
public:
    static void Init();
    static std::vector<Packet*>& GetCreationPackets();

    static void HandleDirtyEntities();
};

#endif
