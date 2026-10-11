#include "pch.h"
#include "PuntDeInteresBotigaSolucio.h"


PuntDeInteresBotigaSolucio::PuntDeInteresRestaurantSolucio(XmlElement tags, std::string name)
{
    m_name = name;
    std::vector<PAIR_ATTR_VALUE>::iterator it;
    m_cuisine = "undefined";
    for(it = m_tags.atributs.begin(); it != m_tags.atributs.end(); it++)
    {
        if(it.first == "cuisine")
            m_cuisine = it.second;
        else if (it.first == "wheelchair" && it.second == "yes")
            m_wheelchair = true;
    }
}

unsigned int PuntDeInteresBotigaSolucio::getColor()
{
    switch(m_cuisine){
        case "chinese":
            return 0x00FFFF;
        case "pizza":
            if(m_wheelchair)
                return 0x7FFFD4;
        default:
            if(m_wheelchair)
                return 0x5D3FD3;
            return PuntDeInteresBase::getColor();
    }
}
