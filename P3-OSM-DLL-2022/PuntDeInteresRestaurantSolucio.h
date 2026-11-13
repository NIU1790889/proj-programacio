#pragma once
#include "PuntDeInteresBase.h"

class PuntDeInteresRestaurantSolucio : public PuntDeInteresBase 
{
    public:
        PuntDeInteresRestaurantSolucio(): PuntDeInteresBase(), m_cuisine(""), m_wheelchair(false){}
        PuntDeInteresRestaurantSolucio(std::string cuisine, bool wheelchair, std::string name, Coordinate coord): PuntDeInteresBase(coord, name), m_cuisine(cuisine), m_wheelchair(wheelchair) {}
        unsigned int getColor() override;
    private:
        std::string m_cuisine;
        bool m_wheelchair;
};