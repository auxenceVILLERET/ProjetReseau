#ifndef MANAGERMETHODS_H_DEFINED
#define MANAGERMETHODS_H_DEFINED
#include <vector>

#include "Packet.h"

class ManagerMethods
{
    static void Init();
    static std::vector<Packet*>& GetCreationPackets();
};

#endif
