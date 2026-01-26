#ifndef CREATE_ENTITY_PACKET_HPP_DEFINED
#define CREATE_ENTITY_PACKET_HPP_DEFINED

#include "Packet.h"
#include "LibGameplay/Gameplay.h"

class CreateEntity : public Packet
{
public:
    uint32_t id;
    EntityType type;
    float x;
    float y;
    float z;
    float dx;
    float dy;
    float dz;
    float scale;

    CreateEntity()
    {
        id = 0;
        type = EntityType::PLAYER;
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
        dx = 0.0f;
        dy = 0.0f;
        dz = 0.0f;
        scale = 1.0f;

        m_type = CREATE_ENTITY;
        m_size = 2 * sizeof(int) + sizeof(id) + sizeof(type) + 7 * sizeof(float);
    }
    CreateEntity(uint32_t _id, EntityType _type, XMFLOAT3 pos = {0.0f, 0.0f, 0.0f}, XMFLOAT3 dir = {0.0f, 0.0f, 0.0f}, float _scale = 1.0f)
    {
        id = _id;
        type = _type;
        x = pos.x;
        y = pos.y;
        z = pos.z;
        dx = dir.x;
        dy = dir.y;
        dz = dir.z;
        scale = _scale;
        
        m_type = CREATE_ENTITY;
        m_size = 2 * sizeof(int) + sizeof(id) + sizeof(type) + 7 * sizeof(float);
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

        std::memcpy(bufferCursor, &id, sizeof(id));
        bufferCursor += sizeof(id);
        std::memcpy(bufferCursor, &type, sizeof(type));
        bufferCursor += sizeof(type);
        
        std::memcpy(bufferCursor, &x, sizeof(x));
        bufferCursor += sizeof(x);
        std::memcpy(bufferCursor, &y, sizeof(y));
        bufferCursor += sizeof(y);
        std::memcpy(bufferCursor, &z, sizeof(z));
        bufferCursor += sizeof(z);
        std::memcpy(bufferCursor, &dx, sizeof(dx));
        bufferCursor += sizeof(dx);
        std::memcpy(bufferCursor, &dy, sizeof(dy));
        bufferCursor += sizeof(dy);
        std::memcpy(bufferCursor, &dz, sizeof(dz));
        bufferCursor += sizeof(dz);
        
        std::memcpy(bufferCursor, &scale, sizeof(scale));
        bufferCursor += sizeof(scale);

        return buffer;
    }
    void Deserialize(char* _message)
    {
        Packet::Deserialize(_message);
        _message += sizeof(m_type) + sizeof(m_size);

        std::memcpy(&id, _message, sizeof(id));
        _message += sizeof(id);
        std::memcpy(&type, _message, sizeof(type));
        _message += sizeof(type);
        
        std::memcpy(&x, _message, sizeof(x));
        _message += sizeof(x);
        std::memcpy(&y, _message, sizeof(y));
        _message += sizeof(y);
        std::memcpy(&z, _message, sizeof(z));
        _message += sizeof(z);
        std::memcpy(&dx, _message, sizeof(dx));
        _message += sizeof(dx);
        std::memcpy(&dy, _message, sizeof(dy));
        _message += sizeof(dy);
        std::memcpy(&dz, _message, sizeof(dz));
        _message += sizeof(dz);
        std::memcpy(&scale, _message, sizeof(scale));
        _message += sizeof(scale);
    }

    void PrintInfo(bool isSent)
    {
        // Packet::PrintInfo(isSent);
    }
};

#endif