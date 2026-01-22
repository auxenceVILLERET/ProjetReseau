#ifndef CLIENTMETHODS_H_DEFINED
#define CLIENTMETHODS_H_DEFINED
#include "Entity.h"
#include "LibNetwork/Packets.hpp"

class ClientMethods 
{
public:
    static Entity* CopyEntity(CreateEntity* entityPacket);

};

#endif
