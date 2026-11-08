#include "PuntDeInteresBotigaSolucio.h"

PuntDeInteresBotigaSolucio::PuntDeInteresBotigaSolucio(XmlElement tags, std::string name)
{
    m_name = name;
    std::vector<PAIR_ATTR_VALUE>::iterator it;
    bool bakery;
    m_shop = "undefined";
    for(it = m_tags.atributs.begin(); it != m_tags.atributs.end() && !bakery; it++)
    {
        if(it.first == "shop")
        {
            m_shop = it.second;
            if (shop == "bakery")
                bakery = true;
        }
        else if (it.first == "wheelchair" && it.second == "yes")
            m_wheelchair = true;
        else if (it.first == "opening_hours" && it.second.find(06:00 - 22:00) != string::n_pos)
            m_hours = true;
    }
}


unsigned int PuntDeInteresBotigaSolucio::getColor()
{
    switch(m_shop){
        case Shop::SUPERMARKET:
            return 0xDFFF00;
        case Shop::TOBACCO:
            return 0xFF7F50 ;
        case Shop::BAKERY:
            if(wheelchair && hours)
                return 0x4CBB17;
            else
                return 0xFA8072;
        case Shop::OTHER:
            return PuntDeInteresBase::getColor();
        default:
            return 0xFFEA00;
    }
}