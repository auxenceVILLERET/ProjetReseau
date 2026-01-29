#ifndef SET_ACTIVE_SHIELD_PACKET_HPP_DEFINED
#define SET_ACTIVE_SHIELD_PACKET_HPP_DEFINED

#include "Packet.h"

class SetActiveShieldPacket : public Packet
{
public:
	uint32_t m_id;
	bool m_active;

	SetActiveShieldPacket()
	{
		m_type = PacketType::SET_ACTIVE_SHIELD;
		m_id = 0;
		m_active = false;
	}

	SetActiveShieldPacket(uint32_t _id, bool _active)
	{
		m_type = PacketType::SET_ACTIVE_SHIELD;
		m_id = _id;
		m_active = _active;
	}

};


#endif // !SET_ACTIVE_SHIELD_PACKET_HPP_DEFINED
