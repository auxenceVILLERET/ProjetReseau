#ifndef CLIENTMETHODS_H_DEFINED
#define CLIENTMETHODS_H_DEFINED

#include "Entity.h"
#include "LibNetwork/Packets/Packets.h"

struct ChatLine
{
	std::string user;
	std::string text;
};

class ClientMethods 
{
public:
    static Entity* CopyEntity(CreateEntity* entityPacket);
	static std::vector<ChatLine> s_chatMessages;

	static bool MoveEntity(uint32_t id, XMFLOAT3 position);
	static bool RotateEntity(uint32_t id, XMFLOAT3 rotation);
	static bool ScaleEntity(uint32_t id, float n);
	static bool ChangePlayerSpeed(uint32_t id, float delta);
	static bool ShootProjectile(XMFLOAT3 position, XMFLOAT3 direction);
	static bool SetActiveState(uint32_t id, bool isActive);
	static bool SetPosition(uint32_t id, XMFLOAT3 position);
	static bool SetDirection(uint32_t id, XMFLOAT3 rotation);
	static bool SendChatMessage(uint32_t id, const char* message);
	static bool AddChatMessage(const std::string& user,const std::string& msg);
};

#endif
