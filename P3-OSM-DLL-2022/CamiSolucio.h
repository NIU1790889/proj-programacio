#pragma once
#include "pch.h"

#include "CamiBase.h"

class CamiSolucio : virtual public CamiBase
{
public:
	std::vector<Coordinate> getCamiCoords() override;
protected:
	std::vector<Coordinate> m_cami;
};

