#pragma once
#include "pch.h"

#include "MapaSolucio.h"

void MapaSolucio::getPdis(std::vector<PuntDeInteresBase*>& pdis)
{

}

void MapaSolucio::getCamins(std::vector<CamiBase*>& camins)
{
	CamiSolucio c;
	camins.clear();
	camins.push_back(&c);
}

void MapaSolucio::parsejaXmlElements(std::vector<XmlElement>& xmlElements)
{

}