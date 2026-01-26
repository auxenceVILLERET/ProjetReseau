#ifndef CHAT_MESSAGE_PACKET_HPP_DEFINED	
#define CHAT_MESSAGE_PACKET_HPP_DEFINED	

#include "Packet.h"

class ChatMessagePacket : public Packet
{
public:
	uint32_t id;
	char message[128];
	ChatMessagePacket()
	{
		id = 0;
		std::memset(message, 0, sizeof(message));
		m_type = CHAT_MESSAGE;
		m_size = 2 * sizeof(int) + sizeof(id) + sizeof(message);
	}

	ChatMessagePacket(uint32_t _id, const char* _message)
	{
		id = _id;
		std::memset(message, 0, sizeof(message));
		strncpy_s(message, sizeof(message), _message, sizeof(message) - 1);
		m_type = CHAT_MESSAGE;
		m_size = 2 * sizeof(int) + sizeof(id) + sizeof(message);
	}

	char* Serialize() override
	{
		char* data = new char[m_size];
		int offset = 0;
		std::memcpy(data + offset, &m_size, sizeof(m_size));
		offset += sizeof(m_size);
		std::memcpy(data + offset, &m_type, sizeof(m_type));
		offset += sizeof(m_type);
		std::memcpy(data + offset, &id, sizeof(id));
		offset += sizeof(id);
		std::memcpy(data + offset, &message, sizeof(message));
		offset += sizeof(message);
		return data;
	}

	void Deserialize(char* data) override
	{
		int offset = 0;
		std::memcpy(&m_size, data + offset, sizeof(m_size));
		offset += sizeof(m_size);
		std::memcpy(&m_type, data + offset, sizeof(m_type));
		offset += sizeof(m_type);
		std::memcpy(&id, data + offset, sizeof(id));
		offset += sizeof(id);
		std::memcpy(&message, data + offset, sizeof(message));
		offset += sizeof(message);
	}

	void PrintInfo(bool isSent)
	{
		Packet::PrintInfo(isSent);
	}
};

#endif // !CHAT_MESSAGE_PACKET_HPP
