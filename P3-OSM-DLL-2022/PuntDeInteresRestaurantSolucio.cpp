#pragma once
#include "pch.h"

#include "PuntDeInteresRestaurantSolucio.h"
PuntDeInteresRestaurantSolucio::PuntDeInteresRestaurantSolucio(XmlElement tags, std::string name)
{
    //m_name = name;
    std::vector<PAIR_ATTR_VALUE>::iterator it;
    m_cuisine = Cuisine::UNDEFINED;
    for(it = tags.atributs.begin(); it != tags.atributs.end(); it++)
    {
        if (it->first == "cuisine")
        {
            if (it->second == "chinese")
                m_cuisine = Cuisine::CHINESE;
            else if (it->second == "pizza")
                m_cuisine = Cuisine::PIZZA;
            else
                m_cuisine = Cuisine::UNDEFINED;
        }
        else if (it->first == "wheelchair" && it->second == "yes")
            m_wheelchair = true;
    }
}


unsigned int PuntDeInteresRestaurantSolucio::getColor()
{
    switch(m_cuisine){
        case Cuisine::CHINESE:
            return 0x00FFFF;
        case Cuisine::PIZZA:
            if(m_wheelchair)
                return 0x7FFFD4;
        default:
            if(m_wheelchair)
                return 0x5D3FD3;
            return PuntDeInteresBase::getColor();
    }
}
