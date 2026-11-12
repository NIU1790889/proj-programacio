#pragma once
#include "PuntDeInteresBase.h"
#include "MapaSolucio.h"

class PuntDeInteresRestaurantSolucio : public PuntDeInteresBase 
{
    public:
        PuntDeInteresRestaurantSolucio(): PuntDeInteresBase(), m_color(0xFFA500) {}
        PuntDeInteresRestaurantSolucio(unsigned int color, std::string name, Coordinate coord): PuntDeInteresBase(coord, name), m_color(color) {}
        unsigned int getColor() {return m_color;}
    private:
        unsigned int m_color;
};