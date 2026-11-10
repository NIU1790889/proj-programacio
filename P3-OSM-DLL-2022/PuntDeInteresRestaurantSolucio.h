#pragma once
#include "PuntDeInteresBase.h"

enum class Cuisine
{
    PIZZA,
    CHINESE,
    UNDEFINED
};

class PuntDeInteresRestaurantSolucio : public PuntDeInteresBase 
{
    public:
        PuntDeInteresRestaurantSolucio(): PuntDeInteresBase(), m_cuisine(Cuisine::UNDEFINED), m_wheelchair(false) {}
        PuntDeInteresRestaurantSolucio(XmlElement tags, std::string name);
        //std::string getName(){return m_name;}
        unsigned int getColor();
    private:
        Cuisine m_cuisine;
        bool m_wheelchair;
};