#ifndef SET_PLAYER_SPEED_PACKET_HPP_DEFINED
#define SET_PLAYER_SPEED_PACKET_HPP_DEFINED

#include "Packet.h"

class SetPlayerSpeedPacket : public Packet
{
public:
    uint32_t id;
    float speed;

    SetPlayerSpeedPacket()
    {
        id = 0;
        speed = 1.0f;

        m_type = SET_PLAYER_SPEED;
        m_size = 2 * sizeof(int) + sizeof(id) + sizeof(float);
    }
    SetPlayerSpeedPacket(uint32_t _id, float _speed)
    {
        id = _id;
        speed = _speed;
        m_type = SET_PLAYER_SPEED;
        m_size = 2 * sizeof(int) + sizeof(id) + sizeof(float);
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
        std::memcpy(bufferCursor, &speed, sizeof(speed));
        bufferCursor += sizeof(speed);

        return buffer;
    }
    void Deserialize(char* _message)
    {
        Packet::Deserialize(_message);
        _message += sizeof(m_type) + sizeof(m_size);
        std::memcpy(&id, _message, sizeof(id));
        _message += sizeof(id);
        std::memcpy(&speed, _message, sizeof(speed));
        _message += sizeof(speed);
    }

    void PrintInfo(bool isSent) {}
};


#endif