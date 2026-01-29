#ifndef CHANGE_COLOR_PARTICLE_PACKET_DEFINED
#define CHANGE_COLOR_PARTICLE_PACKET_DEFINED
#include "Packet.h"

class ChangeColorParticlePacket : public Packet
{
	public:
	uint32_t id;
	int index;
	ChangeColorParticlePacket()
	{
		id = 0;
		index = 0;
		m_type = CHANGE_COLOR_PARTICLE;
		m_size = 2 * sizeof(int) + sizeof(id) + sizeof(index);
	}
	ChangeColorParticlePacket(uint32_t _id, int _index)
	{
		id = _id;
		index = _index;
		m_type = CHANGE_COLOR_PARTICLE;
		m_size = 2 * sizeof(int) + sizeof(id) + sizeof(index);
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
		std::memcpy(bufferCursor, &index, sizeof(index));
		bufferCursor += sizeof(index);
		return buffer;
	}
	void Deserialize(char* _message)
	{
		Packet::Deserialize(_message);
		_message += sizeof(m_type) + sizeof(m_size);
		
		std::memcpy(&id, _message, sizeof(id));
		_message += sizeof(id);
		std::memcpy(&index, _message, sizeof(index));
		_message += sizeof(index);
	}
	
	void PrintInfo(bool isSent)
	{
		// Packet::PrintInfo(isSent);
	}
};

#endif // !CHANGE_COLOR_PARTICLE_PACKET_DEFINED
