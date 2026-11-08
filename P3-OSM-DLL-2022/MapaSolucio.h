#pragma once
#include "pch.h"

#include "MapaBase.h"
#include "Hash.h"



class MapaSolucio : public MapaBase {

public:
	void getPdis(std::vector<PuntDeInteresBase*>& pdis) override;
	void getCamins(std::vector<CamiBase*>& camins) override;
	void parsejaXmlElements(std::vector<XmlElement>& xmlElements) override;

private:
	Hash<int, CamiBase*> m_camins;
	Hash<int, PuntDeInteresBase*> m_puntsInteres;
	Hash<int, Coordinate> m_nodesVia;
};
