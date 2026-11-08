#pragma once
#include "pch.h"

#include "CamiBase.h"

class CamiSolucio : virtual public CamiBase
{
public:
	std::vector<Coordinate> getCamiCoords() override;
	void getCamiCoords(const std::vector<Coordinate>& coords) { m_cami = coords; }
protected:
	std::vector<Coordinate> m_cami;
};

