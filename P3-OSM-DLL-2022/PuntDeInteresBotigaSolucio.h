#pragma once
#include "PuntDeInteresBase.h"
class PuntDeInteresBotigaSolucio: public PuntDeInteresBase 
{
    public:
        PuntDeInteresBotigaSolucio(): m_name("undefined"){}
        PuntDeInteresBotigaSolucio(XmlElement tags, std::string name);
        std::string getName(){return m_name;}
        unsigned int getColor();
    prvate:
        std::string m_name;
        std::string m_shop;
        bool hours;
        bool wheelchair;
};