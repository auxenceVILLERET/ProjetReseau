#ifndef PACKETS_HPP_DEFINED
#define PACKETS_HPP_DEFINED

#include <cstring>
#include <string>

#include "Packet.h"

enum PacketType
{
    BALL_UPDATE,
    MESSAGE,
    PING_PONG,
};

class BallUpdatePacket : public Packet
{
public:
    float x;
    float y;
    float z;
    
    BallUpdatePacket() : Packet()
    {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
        m_size = 2 * sizeof(int) + 3 * sizeof(float);
        m_type = BALL_UPDATE;
    }
    BallUpdatePacket(float _x, float _y, float _z)
    {
        x = _x;
        y = _y;
        z = _z;
        m_size = 2 * sizeof(int) + 3 * sizeof(float);
        m_type = BALL_UPDATE;
    }

    char* Serialize() override
    {
        char* buffer = new char[m_size];
        char* bufferCursor = buffer;
        std::memset(bufferCursor, 0, m_size);
        
        std::memcpy(bufferCursor, &m_type, sizeof(m_type));
        bufferCursor += sizeof(m_type);
        std::memcpy(bufferCursor, &m_size, sizeof(m_size));
        bufferCursor += sizeof(m_size);

        std::memcpy(bufferCursor, &x, sizeof(x));
        bufferCursor += sizeof(x);
        std::memcpy(bufferCursor, &y, sizeof(y));
        bufferCursor += sizeof(y);
        std::memcpy(bufferCursor, &z, sizeof(z));
        bufferCursor += sizeof(z);

        return buffer;
    }

    void Deserialize(char* message) override
    {
        Packet::Deserialize(message);
        message += sizeof(m_type) + sizeof(m_size);
        std::memcpy(&x, message, sizeof(x));
        message += sizeof(x);
        std::memcpy(&y, message, sizeof(y));
        message += sizeof(y);
        std::memcpy(&z, message, sizeof(z));
        message += sizeof(z);
    }
};

class MessagePacket : public Packet
{
public:
    uint32_t len;
    std::string text;
    
    MessagePacket()
    {
        len = 0;
    }
    MessagePacket(std::string _text) : text(_text)
    {
        m_type = MESSAGE;

        len = text.length() + 1;
        m_size = 2 * sizeof(int) + sizeof(uint32_t) + len;
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

        std::memcpy(&len, _message, sizeof(len));
        _message += sizeof(len);
        
        text.assign(_message, len - 1);
        _message += len;
    }
};

class PingPongPacket : public Packet
{
public:
    uint32_t usernameLen;
    std::string username;
    bool isPing = true;
    
    PingPongPacket()
    {
        username = "";
        usernameLen = username.length() + 1;
        isPing = false;

        m_type = PING_PONG;
        
        m_size = 2 * sizeof(int) + sizeof(usernameLen) + usernameLen + sizeof(isPing);
    }
    PingPongPacket(std::string _username, bool _isPing)
    {
        username = _username;
        usernameLen = username.length() + 1;
        isPing = _isPing;

        m_type = PING_PONG;

        m_size = 2 * sizeof(int) + sizeof(usernameLen) + usernameLen + sizeof(isPing);
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
};

#endif
