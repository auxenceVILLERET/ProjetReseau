#ifndef SET_ENTITY_SCALE_HPP_DEFINED
#define SET_ENTITY_SCALE_HPP_DEFINED

#include "Packet.h"

class SetEntityScale : public Packet
{
public:
    uint32_t id;
    float n;

    SetEntityScale()
    {
        id = 0;
        n = 0.0f;

        m_type = SET_ENTITY_POS;
        m_size = 2 * sizeof(int) + sizeof(id) + 3 * sizeof(float);
    }
    SetEntityScale(uint32_t _id, float _n)
    {
        id = _id;
        n = _n;
        
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
        std::memcpy(bufferCursor, &n, sizeof(n));
        bufferCursor += sizeof(n);

        return buffer;
    }
    void Deserialize(char* _message)
    {
        Packet::Deserialize(_message);
        _message += sizeof(m_type) + sizeof(m_size);
        
        std::memcpy(&id, _message, sizeof(id));
        _message += sizeof(id);
        std::memcpy(&n, _message, sizeof(n));
        _message += sizeof(n);
    }

    void PrintInfo(bool isSent) {}
};

#endif
