#pragma once
#include "PuntDeInteresBase.h"
#include "MapaSolucio.h"

class PuntDeInteresRestaurantSolucio : public PuntDeInteresBase 
{
    public:
        PuntDeInteresRestaurantSolucio(): PuntDeInteresBase() {}
        PuntDeInteresRestaurantSolucio(std::string cuisine, bool wheelchair, std::string name, Coordinate coord): PuntDeInteresBase(coord, name), m_cuisine(cuisine), m_wheelchair(wheelchair) {}
        unsigned int getColor();
    private:
        std::string m_cuisine;
        bool m_wheelchair;
};