#pragma once
#include "PuntDeInteresBase.h"
class PuntDeInteresRestaurantSolucio : public PuntDeInteresBase 
{
    public:
        PuntDeInteresRestaurantSolucio (): m_name("undefined"){}
        PuntDeInteresRestaurantSolucio(XmlElement tags, std::string name);
        std::string getName(){return m_name;}
        unsigned int getColor();
    prvate:
        std::string m_name;
        std::string m_cuisine;
        bool m_wheelchair
};