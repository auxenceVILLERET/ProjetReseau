#ifndef CLIENTMETHODS_H_DEFINED
#define CLIENTMETHODS_H_DEFINED
#include "Entity.h"
#include "LibNetwork/Packets.hpp"

class ClientMethods 
{
public:
    static Entity* CopyEntity(CreateEntity* entityPacket);
	
	static bool MoveEntity(uint32_t id, XMFLOAT3 position);
	static bool RotateEntity(uint32_t id, XMFLOAT3 rotation);
	static bool ScaleEntity(uint32_t id, float n);
	static bool ChangePlayerSpeed(uint32_t id, float delta);
	static bool ShootProjectile(uint32_t id, XMFLOAT3 position, XMFLOAT3 direction);
};

#endif
