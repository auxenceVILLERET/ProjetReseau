#ifndef SET_PLAYER_STATS_PACKET_HPP_DEFINED
#define SET_PLAYER_STATS_PACKET_HPP_DEFINED

#include "Packet.h"

class SetPlayerStatsPacket : public Packet
{
public:
    uint32_t id;
    int kills;
    int deaths;
    int score;

    SetPlayerStatsPacket()
    {
        id = 0;
        kills = 0;
        deaths = 0;
        score = 0;

        m_type = SET_PLAYER_STATS;
        m_size = 2 * sizeof(int) + sizeof(uint32_t) + 3 * sizeof(int);
    }
    SetPlayerStatsPacket(uint32_t _id, int _kills, int _deaths, int _score)
    {
        id = _id;
        kills = _kills;
        deaths = _deaths;
        score = _score;

        m_type = SET_PLAYER_STATS;
        m_size = 2 * sizeof(int) + sizeof(uint32_t) + 3 * sizeof(int);
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
        std::memcpy(bufferCursor, &kills, sizeof(kills));
        bufferCursor += sizeof(kills);
        std::memcpy(bufferCursor, &deaths, sizeof(deaths));
        bufferCursor += sizeof(deaths);
        std::memcpy(bufferCursor, &score, sizeof(score));
        bufferCursor += sizeof(score);

        return buffer;
    }

    void Deserialize(char* _message)
    {
        Packet::Deserialize(_message);
        _message += sizeof(m_type) + sizeof(m_size);

        std::memcpy(&id, _message, sizeof(id));
        _message += sizeof(id);
        std::memcpy(&kills, _message, sizeof(kills));
        _message += sizeof(kills);
        std::memcpy(&deaths, _message, sizeof(deaths));
        _message += sizeof(deaths);
        std::memcpy(&score, _message, sizeof(score));
        _message += sizeof(score);
    }
};

#endif