#ifndef SET_ENTITY_HEALTH_PACKET_HPP_DEFINED
#define SET_ENTITY_HEALTH_PACKET_HPP_DEFINED

#include "Packet.h"

class SetEntityHealthPacket : public Packet
{
public:
    uint32_t id;
    float health;
    SetEntityHealthPacket()
    {
        id = 0;
        health = 100.0f;
        m_type = SET_HEALTH;
        m_size = 2 * sizeof(int) + sizeof(id) + sizeof(health);
    }

    SetEntityHealthPacket(uint32_t _id, float _health)
    {
        id = _id;
        health = _health;
        m_type = SET_HEALTH;
        m_size = 2 * sizeof(int) + sizeof(id) + sizeof(health);
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
        std::memcpy(bufferCursor, &health, sizeof(health));
        bufferCursor += sizeof(health);
        return buffer;
    }

    void Deserialize(char* _message)
    {
        Packet::Deserialize(_message);
        _message += sizeof(m_type) + sizeof(m_size);
        std::memcpy(&id, _message, sizeof(id));
        _message += sizeof(id);
        std::memcpy(&health, _message, sizeof(health));
        _message += sizeof(health);
    }

    void PrintInfo(bool isSent)
    {
        // Packet::PrintInfo(isSent);
    }
};

#endif