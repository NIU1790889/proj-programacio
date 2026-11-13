#pragma once
#include "pch.h"

#include "CamiBase.h"

class CamiSolucio : virtual public CamiBase
{
public:
	CamiSolucio(const std::vector<Coordinate>& coords): m_cami(coords) {}
	std::vector<Coordinate> getCamiCoords() override;
protected:
	std::vector<Coordinate> m_cami;
};

