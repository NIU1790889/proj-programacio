#pragma once
#include "pch.h"

#include "MapaBase.h"
#include "Hash.h"
#include "CamiSolucio.h"
#include "PuntDeInteresBotigaSolucio.h"
#include "PuntDeInteresRestaurantSolucio.h"
#include <unordered_map> //delete?



class MapaSolucio : public MapaBase {

public:
	void getPdis(std::vector<PuntDeInteresBase*>& pdis) override;
	void getCamins(std::vector<CamiBase*>& camins) override;
	void parsejaXmlElements(std::vector<XmlElement>& xmlElements) override;

private:
	std::vector<CamiBase*> m_camins;
	std::vector<PuntDeInteresBase*> m_puntsInteres;
	std::unordered_map<std::string, Coordinate> m_nodesVia;
};
