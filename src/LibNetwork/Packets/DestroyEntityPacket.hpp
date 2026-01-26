#ifndef DESTROY_ENTITY_PACKET_HPP_DEFINED
#define DESTROY_ENTITY_PACKET_HPP_DEFINED

#include "Packet.h"

class DestroyEntityPacket : public Packet
{
public:
    uint32_t id;

    DestroyEntityPacket()
    {
        id = 0;
        m_type = DESTROY_ENTITY;
        m_size = 2 * sizeof(int) + sizeof(id);
    }
    DestroyEntityPacket(uint32_t _id)
    {
        id = _id;
        m_type = DESTROY_ENTITY;
        m_size = 2 * sizeof(int) + sizeof(id);
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

        return buffer;
    }

    void Deserialize(char* _message)
    {
        Packet::Deserialize(_message);
        _message += sizeof(m_type) + sizeof(m_size);
        std::memcpy(&id, _message, sizeof(id));
        _message += sizeof(id);
    }

    void PrintInfo(bool isSent)
    {
        Packet::PrintInfo(isSent);
    }

};

#endif