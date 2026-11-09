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

	//futur
	//camins = m_camins; pero nomes els ocupats
}

void MapaSolucio::parsejaXmlElements(std::vector<XmlElement>& xmlElements)
{
	std::vector<XmlElement>::iterator it;

	for (it = xmlElements.begin(); it != xmlElements.end(); it++)
	{
		//agafar id de l'element per utilitzar-ho de clau al hash
		int id = 0; //0 sera el valor per defecte
		int atr = 0;
		while (atr < it->atributs.size() && id == -1)
		{
			if (it->atributs[atr].first == "id")
				id = int(std::stod(it->atributs[atr].second));
			atr++;
		}

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
				//guardar punts d'interes
			}
			else
			{
				//guardar nodes via per facilitar buscar les coords pels camins
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
						//com que els nodes van abans que els camins la informacio ja estara desada
						v.push_back(m_nodesVia[stoi(Util::kvDeTag(it->fills[i].second).second)]);
				}

				//guardar cami a la llista de camisolucio
				CamiSolucio* cami = new CamiSolucio;
				cami->setCamiCoords(v);
				m_camins.insert(id, cami);
			}			
		}

		
	}
}

