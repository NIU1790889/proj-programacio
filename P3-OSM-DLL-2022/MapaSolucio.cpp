#pragma once
#include "pch.h"
#include "Util.h"
#include "MapaSolucio.h"

void MapaSolucio::getPdis(std::vector<PuntDeInteresBase*>& pdis)
{
	//pdis = m_puntsInteres
}

void MapaSolucio::getCamins(std::vector<CamiBase*>& camins)
{
	/*CamiSolucio* c = new CamiSolucio();
	camins.clear();
	camins.push_back(c);*/

	camins = m_camins;
}

void MapaSolucio::parsejaXmlElements(std::vector<XmlElement>& xmlElements)
{
	std::vector<XmlElement>::iterator it;

	for (it = xmlElements.begin(); it != xmlElements.end(); it++)
	{
		if (it->id_element == "node") //guardar nodes
		{
			//mirar si es punt d'interes o node de via
			bool puntInteres = false;
			int fill = 0;
			while (fill < it->fills.size() && !puntInteres)
			{
				if (it->fills[fill].first == "tag")
					if (Util::kvDeTag(it->fills[fill].second).first == "name")
						puntInteres = true;
				fill++;
			}

			if (puntInteres)
			{
				std::vector<PAIR_ATTR_VALUE>::iterator it2; //iterator for fills
				//parameters for the pdis
				std::string name;
				Coordinate coord = {it->atributs[7].second, it->atributs[8].second}
				unsigned int color;
				//bunch of checks
				bool wheelchair = false; //wheelchair accesible
    			bool hours = false; //open between 6 and 22
				bool shop = false; // flase is restaurant, true is shop
				bool bakery = false;
				bool pizza = false;
				
				for(it2 = it.fills.begin(); it2 != it.fills.end(); it2++) //we go through it2 and manually check if its the attribute we want
				{
					if(it2->first.second == "name") //name
						name = it2->second.second;
					else if(it2->first.second == "shop") //colors of shops
					{
						shop = true;
						if (it2->second.second == "supermarket")
							color = 0xDFFF00;
						else if (it2->second.second == "tobacco")
							color = 0xFF7F50;
						else if (it2->second.second == "bakery")
						{
							bakery = true;
							color = 0xFA8072;
						}
						else
							color = 0xFFEA00;            
					}
					else if(it2->first.second == "cuisine") //colors of restaurants
					{
						if (it2->second.second == "pizza")
							pizza = true;
						if(it2->second.second == "chinese")
							color = 0x00FFFF;
						else
							color = 0xFFEA00 
					}
					else if (it2->first.second == "wheelchair" && it2->second.second == "yes") //checks
						wheelchair = true;
					else if (it2->first.second == "opening_hours" && it2->second.second.find("06:00 - 22:00") != std::string::npos) // apparently find returns the position or npos if it isnt there, so we just check if it doesnt return npos
						hours = true;
				}

				if(bakery && wheelchair && hours) //special bakery
					color = 0x4CBB17;
				if(!shop && wheelchair)
					color = 0x5D3FD3;
				if(pizza && wheelchair)
					color = 0x7FFFD4;
				if(shop)
					m_puntsInteres.push_back(PuntDeInteresBotigaSolucio(color, name, coord)); //add shop
				else
					m_puntsInteres.push_back(PuntDeInteresBotigaSolucio(color, name, coord)); //add restaurant
			}
			else
			{
				//guardar nodes via per facilitar buscar les coords pels camins

				//agafar id de l'element per utilitzar-ho de clau al hash
				std::string id = "0"; //0 sera el valor per defecte
				int atr = 0;
				while (atr < it->atributs.size() && id == "0") //no agafa l'id
				{
					if (it->atributs[atr].first == "id")
						id = it->atributs[atr].second;
					atr++;
				}

				Coordinate c;
				c.lat = 0;
				c.lon = 0;
				
				for (int i = 0; i < it->atributs.size(); i++)
				{
					if (it->atributs[i].first == "lat")
						c.lat = std::stod(it->atributs[i].second);
					else if (it->atributs[i].first == "lon")
						c.lon = std::stod(it->atributs[i].second);
				}				
				
				m_nodesVia.insert(id, c);
			}
		}
		else if (it->id_element == "way") //per guardar camins a camisolucio
		{
			//ha de mirar que siguin highway
			bool highway = false;
			int fill = 0;
			while (fill < it->fills.size() && !highway)
			{
				if (it->fills[fill].first == "tag")
					if (Util::kvDeTag(it->fills[fill].second).first == "highway")
						highway = true;
				fill++;
			}

			//nomes guardem highways
			if (highway)
			{
				std::vector<Coordinate> v;

				for (int i = 0; i < it->fills.size(); i++) //recorrer fills buscant nodes
				{
					if (it->fills[i].first == "nd")
					{
						//com que els nodes van abans que els camins la informacio ja estara desada
						v.push_back(m_nodesVia[it->fills[i].second[0].second]);
					}

						
				}

				//guardar cami a la llista de camisolucio
				CamiSolucio* cami = new CamiSolucio;
				cami->setCamiCoords(v);
				m_camins.push_back(cami);
			}			
		}
	}
}

