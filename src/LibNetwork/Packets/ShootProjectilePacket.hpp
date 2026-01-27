#ifndef SHOOT_PROJECTILE_PACKET_HPP_DEFINED
#define SHOOT_PROJECTILE_PACKET_HPP_DEFINED

#include "Packet.h"

class ShootProjectilePacket : public Packet
{
public:
    float px;
    float py;
    float pz;

    float dx;
    float dy;
    float dz;

    ShootProjectilePacket()
    {
        px = py = pz = 0.0f;
        dx = dy = dz = 0.0f;

        m_type = SHOOT_PROJECTILE;
        m_size = 2 * sizeof(int) + 6 * sizeof(float);
    }

    ShootProjectilePacket(float _px, float _py, float _pz, float _dx, float _dy, float _dz)
    {
        px = _px;
        py = _py;
        pz = _pz;
        dx = _dx;
        dy = _dy;
        dz = _dz;

        m_type = SHOOT_PROJECTILE;
        m_size = 2 * sizeof(int) + 6 * sizeof(float);
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

        std::memcpy(bufferCursor, &px, sizeof(px));
        bufferCursor += sizeof(px);
        std::memcpy(bufferCursor, &py, sizeof(py));
        bufferCursor += sizeof(py);
        std::memcpy(bufferCursor, &pz, sizeof(pz));
        bufferCursor += sizeof(pz);
        std::memcpy(bufferCursor, &dx, sizeof(dx));
        bufferCursor += sizeof(dx);
        std::memcpy(bufferCursor, &dy, sizeof(dy));
        bufferCursor += sizeof(dy);
        std::memcpy(bufferCursor, &dz, sizeof(dz));
        bufferCursor += sizeof(dz);
        return buffer;
	}

    void Deserialize(char* _message)
    {
        Packet::Deserialize(_message);
        _message += sizeof(m_type) + sizeof(m_size);
        
        std::memcpy(&px, _message, sizeof(px));
        _message += sizeof(px);
        std::memcpy(&py, _message, sizeof(py));
        _message += sizeof(py);
        std::memcpy(&pz, _message, sizeof(pz));
        _message += sizeof(pz);
        std::memcpy(&dx, _message, sizeof(dx));
        _message += sizeof(dx);
        std::memcpy(&dy, _message, sizeof(dy));
        _message += sizeof(dy);
        std::memcpy(&dz, _message, sizeof(dz));
        _message += sizeof(dz);
    }

	void PrintInfo(bool isSent)
    {
        // Packet::PrintInfo(isSent);
    }

};

#endif