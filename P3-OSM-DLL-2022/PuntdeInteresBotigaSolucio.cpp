#include "PuntDeInteresBotigaSolucio.h"

PuntDeInteresBotigaSolucio::PuntDeInteresBotigaSolucio(XmlElement tags, std::string name)
{
    m_name = name;
    m_wheelchair = false;
    m_hours = false;
    std::vector<PAIR_ATTR_VALUE>::iterator it;
    bool bakery = true;
    m_shop = Shop::UNDEFINED;
    for(it = tags.atributs.begin(); it != tags.atributs.end() && !bakery; it++)
    {
        if(it->first == "shop")
        {
            if (it->second == "supermarket")
                m_shop = Shop::SUPERMARKET;
            else if (it->second == "tobacco")
                m_shop = Shop::TOBACCO;
            else if (it->second == "bakery")
            {
                m_shop = Shop::BAKERY;
                bakery = true;
            }
            else
                m_shop = Shop::OTHER;                
        }
        else if (it->first == "wheelchair" && it->second == "yes")
            m_wheelchair = true;
        else if (it->first == "opening_hours" && it->second.find("06:00 - 22:00")) //no se si he arreglat o trencat aquesta linia
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
            if(m_wheelchair && m_hours)
                return 0x4CBB17;
            else
                return 0xFA8072;
        case Shop::OTHER:
            return PuntDeInteresBase::getColor();
        default:
            return 0xFFEA00;
    }
}