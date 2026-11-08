#pragma once
#include "pch.h"

#include "MapaBase.h"



class MapaSolucio : public MapaBase {

public:
	void getPdis(std::vector<PuntDeInteresBase*>& pdis) override;
	void getCamins(std::vector<CamiBase*>& camins) override;
	void parsejaXmlElements(std::vector<XmlElement>& xmlElements) override;

private:
	//per passar de id d'un node a les seves coordenades
	Coordinate getNodeCoords(std::vector<XmlElement>& xmlElements, std::string ref);
};
