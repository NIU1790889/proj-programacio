#pragma once
#include "PuntDeInteresBase.h"
#include "MapaSolucio.h"

class PuntDeInteresBotigaSolucio: public PuntDeInteresBase 
{
    public:
        PuntDeInteresBotigaSolucio(): PuntDeInteresBase(), m_shop(""), m_opening_hours(""), m_wheelchair(false){}
        PuntDeInteresBotigaSolucio(std::string shop, std::string opening_hours, bool wheelchair, std::string name, Coordinate coord): PuntDeInteresBase(coord, name), m_shop(shop), m_opening_hours(opening_hours), m_wheelchair(wheelchair) {}
        unsigned int getColor() override;
    private:
        std::string m_shop;
        std::string m_opening_hours;
        bool m_wheelchair;
};