#pragma once
#include "PuntDeInteresBase.h"


enum class Shop
{
    SUPERMARKET,
    TOBACCO,
    BAKERY,
    BAKERY_PLUS,  //maybe we don't use it, it was supposed to be for the bakery with specific hours and access
    OTHER
};

class PuntDeInteresBotigaSolucio: public PuntDeInteresBase 
{
    public:
        PuntDeInteresBotigaSolucio(): PuntDeInteresBase() {}
        PuntDeInteresBotigaSolucio(XmlElement tags, std::string name);
        std::string getName(){return m_name;}
        unsigned int getColor();
    protected:
        Shop m_shop;
        bool hours;
        bool wheelchair;
};