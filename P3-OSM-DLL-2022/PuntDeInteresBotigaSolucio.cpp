#include "pch.h"
#include "PuntDeInteresBotigaSolucio.h"

unsigned int PuntDeInteresBotigaSolucio::getColor()
{
    if (m_shop == "supermarket")
		return 0xDFFF00;
	if (m_shop == "tobacco")
		return 0xFF7F50;
	if (m_shop == "bakery")
	{
	    if(m_wheelchair && m_opening_hours.find("06:00") != std::string::npos && m_opening_hours.find("22:00") != std::string::npos) //special bakery
			return 0x4CBB17;
		return 0xFA8072;
	}
	return PuntDeInteresBase::getColor();
}
