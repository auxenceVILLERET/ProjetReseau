#ifndef SET_ACTIVE_STATE_PACKET_HPP_DEFINED
#define SET_ACTIVE_STATE_PACKET_HPP_DEFINED

#include "Packet.h"

class SetActiveStatePacket : public Packet
{
public:
    uint32_t id;
    bool isActive;

    SetActiveStatePacket()
    {
        id = 0;
        isActive = true;
        m_type = SET_ACTIVE_STATE;
        m_size = 2 * sizeof(int) + sizeof(id) + sizeof(isActive);
    }

    SetActiveStatePacket(uint32_t _id, bool _isActive)
    {
        id = _id;
        isActive = _isActive;
        m_type = SET_ACTIVE_STATE;
        m_size = 2 * sizeof(int) + sizeof(id) + sizeof(isActive);
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
        std::memcpy(bufferCursor, &isActive, sizeof(isActive));
        bufferCursor += sizeof(isActive);
        return buffer;
    }

    void Deserialize(char* _message)
    {
        Packet::Deserialize(_message);
        _message += sizeof(m_type) + sizeof(m_size);
        std::memcpy(&id, _message, sizeof(id));
        _message += sizeof(id);
        std::memcpy(&isActive, _message, sizeof(isActive));
        _message += sizeof(isActive);
    }

    void PrintInfo(bool isSent)
    {
        // Packet::PrintInfo(isSent);
    }

};

#endif