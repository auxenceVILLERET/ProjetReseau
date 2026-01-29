#ifndef CHAT_MESSAGE_PACKET_HPP_DEFINED	
#define CHAT_MESSAGE_PACKET_HPP_DEFINED	

#include "Packet.h"

class ChatMessagePacket : public Packet
{
public:
    size_t len;
    std::string text;
	size_t len_username;
	std::string username;

	ChatMessagePacket()
	{
		len = 0;
		len_username = 0;
		m_type = CHAT_MESSAGE;
		m_size = 2 * sizeof(int) + sizeof(size_t) + (int)len + sizeof(size_t) + (int)len_username;
	}

	ChatMessagePacket(std::string _username, std::string _text) : username(_username), text(_text)
	{
		m_type = CHAT_MESSAGE;
		len = text.length() + 1;
		len_username = username.length() + 1;
		m_size = 2 * sizeof(int) + sizeof(size_t) + (int)len + sizeof(size_t) + (int)len_username;
	}

	char* Serialize()
	{
		char* buffer = new char[m_size];
		char* bufferCursor = buffer;
		std::memset(bufferCursor, 0, m_size);
		std::memcpy(bufferCursor, &m_type, sizeof(m_type));
		bufferCursor += sizeof(m_type);
		std::memcpy(bufferCursor, &m_size, sizeof(m_size));
		bufferCursor += sizeof(m_size);
		std::memcpy(bufferCursor, &len_username, sizeof(len_username));
		bufferCursor += sizeof(len_username);
		std::memcpy(bufferCursor, username.c_str(), len_username);
		bufferCursor += len_username;
		std::memcpy(bufferCursor, &len, sizeof(len));
		bufferCursor += sizeof(len);
		std::memcpy(bufferCursor, text.c_str(), len);
		bufferCursor += len;
		return buffer;
	}

	void Deserialize(char* _message)
	{
		Packet::Deserialize(_message);
		_message += sizeof(m_type) + sizeof(m_size);
		std::memcpy(&len_username, _message, sizeof(len_username));
		_message += sizeof(len_username);
		username.assign(_message, len_username - 1);
		_message += len_username;
		std::memcpy(&len, _message, sizeof(len));
		_message += sizeof(len);
		text.assign(_message, len - 1);
		_message += len;
	}

	void PrintInfo(bool isSent)
	{
		// Packet::PrintInfo(isSent);
	}
};

#endif // !CHAT_MESSAGE_PACKET_HPP
