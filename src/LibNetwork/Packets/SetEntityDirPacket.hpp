#ifndef SET_ENTITY_DIR_PACKET_HPP_DEFINED
#define SET_ENTITY_DIR_PACKET_HPP_DEFINED

#include "Packet.h"

class SetEntityDirPacket : public Packet
{
	public:
	uint32_t id;
	float dx;
	float dy;
	float dz;
	SetEntityDirPacket()
	{
		id = 0;
		dx = 0.0f;
		dy = 0.0f;
		dz = 0.0f;
		m_type = SET_ENTITY_DIR;
		m_size = 2 * sizeof(int) + sizeof(id) + sizeof(dx) + sizeof(dy) + sizeof(dz);
	}
	SetEntityDirPacket(uint32_t _id, float _dx, float _dy, float _dz)
	{
		id = _id;
		dx = _dx;
		dy = _dy;
		dz = _dz;
		m_type = SET_ENTITY_DIR;
		m_size = 2 * sizeof(int) + sizeof(id) + sizeof(dx) + sizeof(dy) + sizeof(dz);
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
		std::memcpy(bufferCursor, &dx, sizeof(dx));
		bufferCursor += sizeof(dx);
		std::memcpy(bufferCursor, &dy, sizeof(dy));
		bufferCursor += sizeof(dy);
		std::memcpy(bufferCursor, &dz, sizeof(dz));
		bufferCursor += sizeof(dz);
		return buffer;
	}
	void Deserialize(char* _message)
	{
		Packet::Deserialize(_message);
		_message += sizeof(m_type) + sizeof(m_size);
		std::memcpy(&id, _message, sizeof(id));
		_message += sizeof(id);
		std::memcpy(&dx, _message, sizeof(dx));
		_message += sizeof(dx);
		std::memcpy(&dy, _message, sizeof(dy));
		_message += sizeof(dy);
		std::memcpy(&dz, _message, sizeof(dz));
		_message += sizeof(dz);
	}

	void PrintInfo(bool isSent)
	{
		Packet::PrintInfo(isSent);
	}
};
#endif // !1
