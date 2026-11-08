#pragma once
#include "pch.h"
#include "Util.h"
#include "MapaSolucio.h"

void MapaSolucio::getPdis(std::vector<PuntDeInteresBase*>& pdis)
{

}

void MapaSolucio::getCamins(std::vector<CamiBase*>& camins)
{
	//segurament s'haura de canviar mes endavant pq nomes funciona pq getCamiCoords te valors fixos
	CamiSolucio c;
	camins.clear();
	camins.push_back(&c);
}

void MapaSolucio::parsejaXmlElements(std::vector<XmlElement>& xmlElements)
{
	std::vector<XmlElement>::iterator it;

	for (it = xmlElements.begin(); it != xmlElements.end(); it++)
	{
		if (it->id_element == "way") //guardar camins a camisolucio
		{
			std::vector<Coordinate> v;

			for (int i = 0; i < it->fills.size(); i++) //recorrer fills buscant nodes
			{
				if (it->fills[i].first == "tag")
					if (Util::kvDeTag(it->fills[i].second).first == "name")
						v.push_back(getNodeCoords(xmlElements, Util::kvDeTag(it->fills[i].second).second));
			}
		}

		else if (it->id_element == "node")
		{
			//guardar punts d'interes
		}
	}
}

Coordinate MapaSolucio::getNodeCoords(std::vector<XmlElement>& xmlElements, std::string ref)
{
	std::vector<XmlElement>::iterator it;

	Coordinate c;
	c.lat = 0;
	c.lon = 0;

	for (it = xmlElements.begin(); it != xmlElements.end(); it++)
	{
		if (it->id_element == "node" && it->atributs[0].second == ref)
		{
			for (int i = 0; i < it->atributs.size(); i++)
			{
				if (it->atributs[i].first == "lat")
					c.lat = std::stod(it->atributs[i].second);
				else if (it->atributs[i].first == "lon")
					c.lon = std::stod(it->atributs[i].second);
			}

			return c;
		}
	}

	return c;
}
