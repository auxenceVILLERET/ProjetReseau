#ifndef SET_ENTITY_ROT_PACKET_HPP_DEFINED
#define SET_ENTITY_ROT_PACKET_HPP_DEFINED

#include "Packet.h"

class SetEntityRot : public Packet
{
public:
    uint32_t id;
    float _11;
    float _12;
    float _13;
    float _21;
    float _22;
    float _23;
    float _31;
    float _32;
    float _33;

    SetEntityRot()
    {
        id = 0;
        _11 = 0.0f;
        _12 = 0.0f;
        _13 = 0.0f;
        _21 = 0.0f;
        _22 = 0.0f;
        _23 = 0.0f;
        _31 = 0.0f;
        _32 = 0.0f;
        _33 = 0.0f;

        m_size = 2 * sizeof(int) + sizeof(id) + 9 * sizeof(float);
        m_type = SET_ENTITY_ROT;
    }
    SetEntityRot(uint32_t _id, XMFLOAT4X4 _rotation)
    {
        id = _id;
        _11 = _rotation._11;
        _12 = _rotation._12;
        _13 = _rotation._13;
        _21 = _rotation._21;
        _22 = _rotation._22;
        _23 = _rotation._23;
        _31 = _rotation._31;
        _32 = _rotation._32;
        _33 = _rotation._33;
        
        m_size = 2 * sizeof(int) + sizeof(id) + 9 * sizeof(float);
        m_type = SET_ENTITY_ROT;
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
        
        std::memcpy(bufferCursor, &_11, sizeof(_11));
        bufferCursor += sizeof(_11);
        std::memcpy(bufferCursor, &_12, sizeof(_12));
        bufferCursor += sizeof(_12);
        std::memcpy(bufferCursor, &_13, sizeof(_13));
        bufferCursor += sizeof(_13);
        std::memcpy(bufferCursor, &_21, sizeof(_21));
        bufferCursor += sizeof(_21);
        std::memcpy(bufferCursor, &_22, sizeof(_22));
        bufferCursor += sizeof(_22);
        std::memcpy(bufferCursor, &_23, sizeof(_23));
        bufferCursor += sizeof(_23);
        std::memcpy(bufferCursor, &_31, sizeof(_31));
        bufferCursor += sizeof(_31);
        std::memcpy(bufferCursor, &_32, sizeof(_32));
        bufferCursor += sizeof(_32);
        std::memcpy(bufferCursor, &_33, sizeof(_33));
        bufferCursor += sizeof(_33);
        
        return buffer;
    }
    void Deserialize(char* _message)
    {
        Packet::Deserialize(_message);
        _message += sizeof(m_type) + sizeof(m_size);

        std::memcpy(&id, _message, sizeof(id));
        _message += sizeof(id);
        std::memcpy(&_11, _message, sizeof(_11));
        _message += sizeof(_11);
        std::memcpy(&_12, _message, sizeof(_12));
        _message += sizeof(_12);
        std::memcpy(&_13, _message, sizeof(_13));
        _message += sizeof(_13);
        std::memcpy(&_21, _message, sizeof(_21));
        _message += sizeof(_21);
        std::memcpy(&_22, _message, sizeof(_22));
        _message += sizeof(_22);
        std::memcpy(&_23, _message, sizeof(_23));
        _message += sizeof(_23);
        std::memcpy(&_31, _message, sizeof(_31));
        _message += sizeof(_31);
        std::memcpy(&_32, _message, sizeof(_32));
        _message += sizeof(_32);
        std::memcpy(&_33, _message, sizeof(_33));
        _message += sizeof(_33);
    }

    void PrintInfo(bool isSent) {}
};

#endif