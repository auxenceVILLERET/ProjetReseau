#ifndef SET_ENTITY_POS_PACKET_HPP
#define SET_ENTITY_POS_PACKET_HPP

#include "Packet.h"

class SetEntityPos : public Packet
{
public:
    uint32_t id;
    float x;
    float y;
    float z;

    SetEntityPos()
    {
        id = 0;
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;

        m_type = SET_ENTITY_POS;
        m_size = 2 * sizeof(int) + sizeof(id) + 3 * sizeof(float);
    }
    SetEntityPos(uint32_t _id, float _x, float _y, float _z)
    {
        id = _id;
        x = _x;
        y = _y;
        z = _z;
        
        m_type = SET_ENTITY_POS;
        m_size = 2 * sizeof(int) + sizeof(id) + 3 * sizeof(float);
    }

    char* Serialize()
    {
        char* buffer = new char[m_size];
        char* bufferCursor = buffer;
        memset(buffer, 0, m_size);

        memcpy(bufferCursor, &m_type, sizeof(m_type));
        bufferCursor += sizeof(m_type);
        std::memcpy(bufferCursor, &m_size, sizeof(m_size));
        bufferCursor += sizeof(m_size);
        
        std::memcpy(bufferCursor, &id, sizeof(id));
        bufferCursor += sizeof(id);
        std::memcpy(bufferCursor, &x, sizeof(x));
        bufferCursor += sizeof(x);
        std::memcpy(bufferCursor, &y, sizeof(y));
        bufferCursor += sizeof(y);
        std::memcpy(bufferCursor, &z, sizeof(z));
        bufferCursor += sizeof(z);

        return buffer;
    }
    void Deserialize(char* _message)
    {
        Packet::Deserialize(_message);
        _message += sizeof(m_type) + sizeof(m_size);
        
        std::memcpy(&id, _message, sizeof(id));
        _message += sizeof(id);
        std::memcpy(&x, _message, sizeof(x));
        _message += sizeof(x);
        std::memcpy(&y, _message, sizeof(y));
        _message += sizeof(y);
        std::memcpy(&z, _message, sizeof(z));
        _message += sizeof(z);
    }

    void PrintInfo(bool isSent)
    {
        // Packet::PrintInfo(bool isSent);
        // std::cout << " x : " << x << ", y : " << y << " , z : " << z << std::endl;
    }
};

#endif