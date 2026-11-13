#include "PuntDeInteresRestaurantSolucio.h"

unsigned int PuntDeInteresRestaurantSolucio::getColor()
{
	if (m_cuisine == "chinese")
		return 0x00FFFF;
    if(m_wheelchair)
    {
        if (m_cuisine == "pizza")
            return 0x7FFFD4;
		return 0x5D3FD3;
    }	
	return 0xFFEA00;
}