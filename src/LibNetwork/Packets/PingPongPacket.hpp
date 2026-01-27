#ifndef PINGPONG_PACKET_HPP_DEFINED
#define PINGPONG_PACKET_HPP_DEFINED

#include "Packet.h"

class PingPongPacket : public Packet
{
public:
    size_t usernameLen;
    std::string username;
    bool isPing = true;
    
    PingPongPacket()
    {
        username = "";
        usernameLen = username.length() + 1;
        isPing = false;

        m_type = PING_PONG;
        
        m_size = 2 * sizeof(int) + sizeof(usernameLen) + (int)usernameLen + sizeof(isPing);
    }
    PingPongPacket(const std::string& _username, bool _isPing)
    {
        username = _username;
        usernameLen = username.length() + 1;
        isPing = _isPing;

        m_type = PING_PONG;

        m_size = 2 * sizeof(int) + sizeof(usernameLen) + (int)usernameLen + sizeof(isPing);
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
        
        std::memcpy(bufferCursor, &usernameLen, sizeof(usernameLen));
        bufferCursor += sizeof(usernameLen);
        std::memcpy(bufferCursor, username.c_str(), usernameLen);
        bufferCursor += usernameLen;
        
        std::memcpy(bufferCursor, &isPing, sizeof(isPing));
        bufferCursor += sizeof(isPing);

        return buffer;
    }
    
    void Deserialize(char* message)
    {
        Packet::Deserialize(message);
        message += sizeof(m_type) + sizeof(m_size);

        std::memcpy(&usernameLen, message, sizeof(usernameLen));
        message += sizeof(usernameLen);
        username.assign(message, usernameLen - 1);
        message += usernameLen;
        
        std::memcpy(&isPing, message, sizeof(isPing));
        message += sizeof(isPing);
    }

    void PrintInfo(bool isSent)
    {
        Packet::PrintInfo(isSent);
    }
};

#endif
