#ifndef SET_POWER_UP_TYPE_PACKET_DEFINED
#define SET_POWER_UP_TYPE_PACKET_DEFINED

#include "Packet.h"
#include "LibGameplay/Gameplay.h"

class SetPowerUpTypePacket : public Packet
{
public:
	uint32_t id;
	PowerUpType powerUpType;

	SetPowerUpTypePacket()
	{
		id = 0;
		powerUpType = PowerUpType::HEALTH;
		m_type = SET_POWER_UP_TYPE;
		m_size = 2 * sizeof(int) + sizeof(id) + sizeof(powerUpType);
	}

	SetPowerUpTypePacket(uint32_t _id, PowerUpType _powerUpType)
	{
		id = _id;
		powerUpType = _powerUpType;
		m_type = SET_POWER_UP_TYPE;
		m_size = 2 * sizeof(int) + sizeof(id) + sizeof(powerUpType);
	}

	char* Serialize()
	{
		char* buffer = new char[m_size];
		char* bufferCursor = buffer;
		std::memset(buffer, 0, m_size);
		std::memcpy(bufferCursor, &m_type, sizeof(m_type));
		bufferCursor += sizeof(m_type);
		std::memcpy(bufferCursor, &m_size, sizeof(m_size));
		bufferCursor += sizeof(m_size);
		std::memcpy(bufferCursor, &id, sizeof(id));
		bufferCursor += sizeof(id);
		std::memcpy(bufferCursor, &powerUpType, sizeof(powerUpType));
		bufferCursor += sizeof(powerUpType);
		return buffer;
	}

	void Deserialize(char* _message)
	{
		Packet::Deserialize(_message);
		_message += sizeof(m_type) + sizeof(m_size);
		std::memcpy(&id, _message, sizeof(id));
		_message += sizeof(id);
		std::memcpy(&powerUpType, _message, sizeof(powerUpType));
		_message += sizeof(powerUpType);
	}

	void PrintInfo(bool isSent)
	{
		// Packet::PrintInfo(isSent);
	}
};


#endif // !SET_POWER_UP_TYPE_PACKET_DEFINED
