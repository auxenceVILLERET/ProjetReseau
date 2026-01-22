#ifndef PACKETS_HPP_DEFINED
#define PACKETS_HPP_DEFINED

#include <cstring>
#include <string>

#include "Packet.h"
#include "LibGameplay/Entity.h"
#include "LibGameplay/Gameplay.h"

enum PacketType
{
    BALL_UPDATE,
    MESSAGE,
    PING_PONG,
    CREATE_ENTITY,
    SET_PLAYER_ID,
    SET_ENTITY_POS,
    SET_ENTITY_ROT,
    SET_ENTITY_SCALE,
    MOVE_ENTITY,
    ROTATE_ENTITY,
    SCALE_ENTITY
};

inline std::vector<std::string> PacketTypeNames
{
    "BALL UPDATE",
    "MESSAGE",
    "PING PONG",
    "CREATE ENTITY",
    "SET PLAYER ID",
    "SET ENTITY POS",
    "SET ENTITY ROT",
    "SET ENTITY SCALE",
    "MOVE ENTITY",
    "ROTATE ENTITY",
    "SCALE ENTITY"
};

class BallUpdatePacket : public Packet
{
public:
    float x;
    float y;
    float z;
    
    BallUpdatePacket() : Packet()
    {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
        m_size = 2 * sizeof(int) + 3 * sizeof(float);
        m_type = BALL_UPDATE;
    }
    BallUpdatePacket(float _x, float _y, float _z)
    {
        x = _x;
        y = _y;
        z = _z;
        m_size = 2 * sizeof(int) + 3 * sizeof(float);
        m_type = BALL_UPDATE;
    }

    char* Serialize() override
    {
        char* buffer = new char[m_size];
        char* bufferCursor = buffer;
        std::memset(bufferCursor, 0, m_size);
        
        std::memcpy(bufferCursor, &m_type, sizeof(m_type));
        bufferCursor += sizeof(m_type);
        std::memcpy(bufferCursor, &m_size, sizeof(m_size));
        bufferCursor += sizeof(m_size);

        std::memcpy(bufferCursor, &x, sizeof(x));
        bufferCursor += sizeof(x);
        std::memcpy(bufferCursor, &y, sizeof(y));
        bufferCursor += sizeof(y);
        std::memcpy(bufferCursor, &z, sizeof(z));
        bufferCursor += sizeof(z);

        return buffer;
    }

    void Deserialize(char* message) override
    {
        Packet::Deserialize(message);
        message += sizeof(m_type) + sizeof(m_size);
        std::memcpy(&x, message, sizeof(x));
        message += sizeof(x);
        std::memcpy(&y, message, sizeof(y));
        message += sizeof(y);
        std::memcpy(&z, message, sizeof(z));
        message += sizeof(z);
    }
};

class MessagePacket : public Packet
{
public:
    uint32_t len;
    std::string text;
    
    MessagePacket()
    {
        len = 0;
    }
    MessagePacket(std::string _text) : text(_text)
    {
        m_type = MESSAGE;

        len = text.length() + 1;
        m_size = 2 * sizeof(int) + sizeof(uint32_t) + len;
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

        std::memcpy(bufferCursor, &len, sizeof(len));
        bufferCursor += sizeof(len);
        
        std::memcpy(bufferCursor, text.c_str(), len);
        bufferCursor += len;

        return buffer;
    }
    void Deserialize(char* _message)
    {
        Packet::Deserialize(_message);
        _message += sizeof(m_type) + sizeof(m_size);

        std::memcpy(&len, _message, sizeof(len));
        _message += sizeof(len);
        
        text.assign(_message, len - 1);
        _message += len;
    }
};

class PingPongPacket : public Packet
{
public:
    uint32_t usernameLen;
    std::string username;
    bool isPing = true;
    
    PingPongPacket()
    {
        username = "";
        usernameLen = username.length() + 1;
        isPing = false;

        m_type = PING_PONG;
        
        m_size = 2 * sizeof(int) + sizeof(usernameLen) + usernameLen + sizeof(isPing);
    }
    PingPongPacket(const std::string& _username, bool _isPing)
    {
        username = _username;
        usernameLen = username.length() + 1;
        isPing = _isPing;

        m_type = PING_PONG;

        m_size = 2 * sizeof(int) + sizeof(usernameLen) + usernameLen + sizeof(isPing);
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
        
        std::memcpy(bufferCursor, &usernameLen, sizeof(usernameLen));
        bufferCursor += sizeof(usernameLen);
        std::memcpy(bufferCursor, username.c_str(), usernameLen);
        bufferCursor += usernameLen;
        
        std::memcpy(bufferCursor, &isPing, sizeof(isPing));
        bufferCursor += sizeof(isPing);

        return buffer;
    }
    
    void Deserialize(char* message)
    {
        Packet::Deserialize(message);
        message += sizeof(m_type) + sizeof(m_size);

        std::memcpy(&usernameLen, message, sizeof(usernameLen));
        message += sizeof(usernameLen);
        username.assign(message, usernameLen - 1);
        message += usernameLen;
        
        std::memcpy(&isPing, message, sizeof(isPing));
        message += sizeof(isPing);
    }
};

class CreateEntity : public Packet
{
public:
    uint32_t id;
    EntityType type;
    float x;
    float y;
    float z;
    float scale;

    CreateEntity()
    {
        id = 0;
        type = EntityType::PLAYER;
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
        scale = 1.0f;

        m_type = CREATE_ENTITY;
        m_size = 2 * sizeof(int) + sizeof(id) + sizeof(type) + 4 * sizeof(float);
    }
    CreateEntity(uint32_t _id, EntityType _type, float _x = 0.0f, float _y = 0.0f, float _z = 0.0f, float _scale = 1.0f)
    {
        id = _id;
        type = _type;
        x = _x;
        y = _y;
        z = _z;
        scale = _scale;
        
        m_type = CREATE_ENTITY;
        m_size = 2 * sizeof(int) + sizeof(id) + sizeof(type) + 4 * sizeof(float);
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
        std::memcpy(&scale, _message, sizeof(scale));
        _message += sizeof(scale);
    }
};

class SetPlayerIDPacket : public Packet
{
public:
    uint32_t id;

    SetPlayerIDPacket()
    {
        id = 0;
        m_type = SET_PLAYER_ID;
        m_size = 2 * sizeof(int) + sizeof(id);
    }
    SetPlayerIDPacket(uint32_t _id)
    {
        id = _id;
        m_type = SET_PLAYER_ID;
        m_size = 2 * sizeof(int) + sizeof(id);
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

        return buffer;
    }

    void Deserialize(char* _message)
    {
        Packet::Deserialize(_message);
        _message += sizeof(m_type) + sizeof(m_size);
        std::memcpy(&id, _message, sizeof(id));
        _message += sizeof(id);
    }
};

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
};

class SetEntityRot : public Packet
{
public:
    uint32_t id;
    float x;
    float y;
    float z;
    float w;

    SetEntityRot()
    {
        id = 0;
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
        w = 0.0f;

        m_size = 2 * sizeof(int) + sizeof(id) + 4 * sizeof(float);
        m_type = SET_ENTITY_ROT;
    }
    SetEntityRot(uint32_t _id, float _x, float _y, float _z, float _w)
    {
        id = _id;
        x = _x;
        y = _y;
        z = _z;
        w = _w;
        
        m_size = 2 * sizeof(int) + sizeof(id) + 4 * sizeof(float);
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
        std::memcpy(bufferCursor, &x, sizeof(x));
        bufferCursor += sizeof(x);
        std::memcpy(bufferCursor, &y, sizeof(y));
        bufferCursor += sizeof(y);
        std::memcpy(bufferCursor, &z, sizeof(z));
        bufferCursor += sizeof(z);
        std::memcpy(bufferCursor, &w, sizeof(w));
        bufferCursor += sizeof(w);
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
        std::memcpy(&z, _message, sizeof(w));
        _message += sizeof(w);
    }
};

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
};

class MoveEntityPacket : public Packet
{
public:
    uint32_t id;
    float x;
    float y;
    float z;

    MoveEntityPacket()
    {
        id = 0;
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;

        m_type = MOVE_ENTITY;
        m_size = 2 * sizeof(int) + sizeof(id) + 3 * sizeof(float);
    }
    MoveEntityPacket(uint32_t _id, float _x, float _y, float _z)
    {
        id = _id;
        x = _x;
        y = _y;
        z = _z;
        
        m_type = MOVE_ENTITY;
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
};

class RotateEntityPacket : public Packet
{
public:
    uint32_t id;
    float x;
    float y;
    float z;

    RotateEntityPacket()
    {
        id = 0;
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;

        m_type = ROTATE_ENTITY;
        m_size = 2 * sizeof(int) + sizeof(id) + 3 * sizeof(float);
    }
    RotateEntityPacket(uint32_t _id, float _x, float _y, float _z)
    {
        id = _id;
        x = _x;
        y = _y;
        z = _z;
        
        m_type = ROTATE_ENTITY;
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
};

class ScaleEntityPacket : public Packet
{
public:
    uint32_t id;
    float n;

    ScaleEntityPacket()
    {
        id = 0;
        n = 1.0f;

        m_type = SCALE_ENTITY;
        m_size = 2 * sizeof(int) + sizeof(id) + sizeof(float);
    }
    ScaleEntityPacket(uint32_t _id, float _n)
    {
        id = _id;
        n = _n;
        
        m_type = SCALE_ENTITY;
        m_size = 2 * sizeof(int) + sizeof(id) + sizeof(float);
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
};

#endif
