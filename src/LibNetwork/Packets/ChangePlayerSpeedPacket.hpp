#ifndef CHANGE_PLAYER_SPEED_PACKET_HPP_DEFINED
#define CHANGE_PLAYER_SPEED_PACKET_HPP_DEFINED

#include "Packet.h"

class ChangePlayerSpeedPacket: public Packet
{
public:
    uint32_t id;
    float delta;

    ChangePlayerSpeedPacket()
    {
        id = 0;
        delta = 0.0f;

        m_type = CHANGE_PLAYER_SPEED;
        m_size = 2 * sizeof(int) + sizeof(id) + sizeof(delta);
    }
    ChangePlayerSpeedPacket(uint32_t _id, float _delta)
    {
        id = _id;
        delta = _delta;

        m_type = CHANGE_PLAYER_SPEED;
        m_size = 2 * sizeof(int) + sizeof(id) + sizeof(delta);
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
        std::memcpy(bufferCursor, &delta, sizeof(delta));
        bufferCursor += sizeof(delta);

        return buffer;
    }

    void Deserialize(char* _message)
    {
        Packet::Deserialize(_message);
        _message += sizeof(m_type) + sizeof(m_size);
        
        std::memcpy(&id, _message, sizeof(id));
        _message += sizeof(id);
        std::memcpy(&delta, _message, sizeof(delta));
        _message += sizeof(delta);
    }
    
    void PrintInfo(bool isSent)
    {
        // Packet::PrintInfo(isSent);
    }
};

#endif