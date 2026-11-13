#include "PuntDeInteresRestaurantSolucio.cpp"

unsigned int PuntDeInteresRestaurantSolucio::getColor()
{
	if (m_cuisine == "chinese")
		return 0x00FFFF;
    if( wheelchair)
    {
        if (m_cuisine == "pizza")
            return 0x7FFFD4;
		return 0x5D3FD3;
    }	
	return 0xFFEA00;
}