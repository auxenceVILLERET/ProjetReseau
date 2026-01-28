#ifndef PACKET_H_DEFINED
#define PACKET_H_DEFINED
#include <string>
#include <vector>

inline std::vector<std::string> PacketTypeNames
{
    "BALL UPDATE",
    "MESSAGE",
    "PING PONG",
    "CREATE ENTITY",
    "SET PLAYER ID",
    "SET ENTITY POS",
    "SET ENTITY ROT",
    "SET ENTITY SCALE",
    "MOVE ENTITY",
    "ROTATE ENTITY",
    "SCALE ENTITY",
    "CHANGE PLAYER SPEED",
    "SET PLAYER SPEED",
    "SHOOT PROJECTILE",
    "DESTROY ENTITY",
	"SET ACTIVE STATE",
    "SET HEALTH",
	"SET ENTITY DIR",
	"CHAT MESSAGE",
    "SET PLAYER STATS",
	"CHANGE COLOR SHIP",
	"CHANGE COLOR PARTICLE",
	"SET POWER UP TYPE",
	"SET PLAYER USERNAME"
};

enum PacketType
{
    BALL_UPDATE,
    MESSAGE,
    PING_PONG,
    CREATE_ENTITY,
    SET_PLAYER_ID,
    SET_ENTITY_POS,
    SET_ENTITY_ROT,
    SET_ENTITY_SCALE,
    MOVE_ENTITY,
    ROTATE_ENTITY,
    SCALE_ENTITY,
    CHANGE_PLAYER_SPEED,
    SET_PLAYER_SPEED,
    SHOOT_PROJECTILE,
    DESTROY_ENTITY,
    SET_ACTIVE_STATE,
    SET_HEALTH,
	SET_ENTITY_DIR,
	CHAT_MESSAGE,
    SET_PLAYER_STATS,
    CHANGE_COLOR_SHIP,
	CHANGE_COLOR_PARTICLE,
	SET_POWER_UP_TYPE,
	SET_PLAYER_USERNAME
};

class Packet
{
public:
    Packet();
    Packet(int size, int type);
    virtual ~Packet() = default;

    int GetType() { return m_type; }
    
    virtual void Deserialize(char* message);
    virtual char* Serialize();

    virtual void PrintInfo(bool isSent);
    
protected:
    int m_size;
    int m_type;

private:
    friend class Message;
};

#endif
