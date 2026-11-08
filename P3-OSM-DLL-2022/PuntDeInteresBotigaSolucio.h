#pragma once
#include "PuntDeInteresBase.h"


enum class Shop
{
    SUPERMARKET,
    TOBACCO,
    BAKERY,
    BAKERY_PLUS,  //maybe we don't use it, it was supposed to be for the bakery with specific hours and access
    OTHER,
    UNDEFINED
};

class PuntDeInteresBotigaSolucio: public PuntDeInteresBase 
{
    public:
        PuntDeInteresBotigaSolucio(): PuntDeInteresBase(), m_shop(Shop::UNDEFINED), m_wheelchair(false), m_hours(false) {}
        PuntDeInteresBotigaSolucio(XmlElement tags, std::string name);
        std::string getName(){return m_name;}
        unsigned int getColor();
    protected:
        Shop m_shop;
        bool m_hours;
        bool m_wheelchair;
};