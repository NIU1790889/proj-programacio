#pragma once
#include "pch.h"
#include "Util.h"
#include "MapaSolucio.h"

void MapaSolucio::getPdis(std::vector<PuntDeInteresBase*>& pdis)
{
	/*std::vector<PuntDeInteresBase*> vpdi;
	vpdi.push_back(new PuntDeInteresRestaurantSolucio( "regional", true, "El Millor Restaurant", {41.4902204, 2.1406477}));
	vpdi.push_back(new PuntDeInteresBotigaSolucio("bakery", "", false, "La Millor Pastisseria", {41.4918606, 2.1465411}));
	pdis = vpdi;*/
	
	pdis = m_puntsInteres;
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
	m_puntsInteres.clear();
	m_camins.clear();
	m_nodesVia = std::unordered_map<std::string, Coordinate>();
	std::vector<XmlElement>::iterator it;

	for (it = xmlElements.begin(); it != xmlElements.end(); it++)
	{
		if (it->id_element == "node") //guardar nodes
		{
			//mirar si es punt d'interes o node de via
			bool name = false, via = false;
			for (int fill = 0; fill < it->fills.size(); fill++)
			{
				if (it->fills[fill].first == "tag")
				{
					std::string key = Util::kvDeTag(it->fills[fill].second).first;

					if (key == "name")
						name = true;
					if (key == "highway" || key == "public_transport" || key == "access" || key == "entrance")
						via = true;					
				}
			}

			if (!name || it->fills.size() == 0 || via)
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

				m_nodesVia.insert({ id, c });
			}

			else
			{
				std::vector<std::pair<std::string, std::vector<std::pair<std::string, std::string>>>>::iterator it2; //iterator for fills
				//parameters for the pdis
				std::string name;
				Coordinate coord;
				coord.lat = 0;
				coord.lon = 0;
				bool wheelchair = false; //wheelchair accesible
    			std::string hours = "";
				std::string category = "";
				for (int i = 0; i < it->atributs.size(); i++)
				{
					if (it->atributs[i].first == "lat")
						coord.lat = std::stod(it->atributs[i].second);
					else if (it->atributs[i].first == "lon")
						coord.lon = std::stod(it->atributs[i].second);
				}
				//bunch of checks


				bool shop = false;
				bool restaurant = false;
				
				for(it2 = it->fills.begin(); it2 != it->fills.end(); it2++) //we go through it2 and manually check if its the attribute we want
				{
					if ((it2->first == "tag") && (Util::kvDeTag(it2->second).first == "name")) //name
					{
						name = Util::kvDeTag(it2->second).second;
					}
					else if((it2->first == "tag") && (Util::kvDeTag(it2->second).first == "shop")) //colors of shops
					{
						shop = true;
						category = Util::kvDeTag(it2->second).second;         
					}
					else if ((it2->first == "tag") && (Util::kvDeTag(it2->second).first == "amenity") && (Util::kvDeTag(it2->second).second == "restaurant"))
						restaurant = true;
					else if((it2->first == "tag") && (Util::kvDeTag(it2->second).first == "cuisine")) //colors of restaurants
						category = Util::kvDeTag(it2->second).second;
					else if (Util::kvDeTag(it2->second).first == "wheelchair" && Util::kvDeTag(it2->second).second == "yes") //checks
						wheelchair = true;
					else if (Util::kvDeTag(it2->second).first == "opening_hours") 
						hours = Util::kvDeTag(it2->second).second;
				}
				if(shop)
					m_puntsInteres.push_back(new PuntDeInteresBotigaSolucio(category, hours, wheelchair, name, coord)); //add shop
				else if (restaurant)
					m_puntsInteres.push_back(new PuntDeInteresRestaurantSolucio(category, wheelchair, name, coord)); //add restaurant
				else 
					m_puntsInteres.push_back(new PuntDeInteresBase(coord, name)); //add base
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
				CamiSolucio* cami = new CamiSolucio(v);
				m_camins.push_back(cami);
			}			
		}
	}
}

