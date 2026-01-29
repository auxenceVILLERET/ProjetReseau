#ifndef SET_ACTIVE_SHIELD_PACKET_HPP_DEFINED
#define SET_ACTIVE_SHIELD_PACKET_HPP_DEFINED

#include "Packet.h"

class SetActiveShieldPacket : public Packet
{
public:
	uint32_t m_id;
	bool m_active;

	SetActiveShieldPacket()
	{
		m_type = PacketType::SET_ACTIVE_SHIELD;
		m_id = 0;
		m_active = false;
	}

	SetActiveShieldPacket(uint32_t _id, bool _active)
	{
		m_type = PacketType::SET_ACTIVE_SHIELD;
		m_id = _id;
		m_active = _active;
	}

	char* Serialize()
	{
		m_size = sizeof(m_type) + sizeof(m_size) + sizeof(m_id) + sizeof(m_active);
		char* buffer = new char[m_size];
		char* bufferCursor = buffer;
		std::memset(buffer, 0, m_size);
		std::memcpy(bufferCursor, &m_type, sizeof(m_type));
		bufferCursor += sizeof(m_type);
		std::memcpy(bufferCursor, &m_size, sizeof(m_size));
		bufferCursor += sizeof(m_size);
		std::memcpy(bufferCursor, &m_id, sizeof(m_id));
		bufferCursor += sizeof(m_id);
		std::memcpy(bufferCursor, &m_active, sizeof(m_active));
		bufferCursor += sizeof(m_active);
		return buffer;
	}

	void Deserialize(char* _message)
	{
		Packet::Deserialize(_message);
		_message += sizeof(m_type) + sizeof(m_size);
		std::memcpy(&m_id, _message, sizeof(m_id));
		_message += sizeof(m_id);
		std::memcpy(&m_active, _message, sizeof(m_active));
		_message += sizeof(m_active);
	}

	void PrintInfo(bool isSent)
	{
		Packet::PrintInfo(isSent);
	}

};


#endif // !SET_ACTIVE_SHIELD_PACKET_HPP_DEFINED
