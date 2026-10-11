#pragma once
#include "pch.h"

#include <string>
#include "PuntDeInteresBase.h"

class PuntDeInteresBotigaSolucio : public PuntDeInteresBase {

private:
	Coordinate m_coord;
	std::string m_name;

public:
	PuntDeInteresBotigaSolucio();
	PuntDeInteresBotigaSolucio(Coordinate coord, std::string name);

	std::string getName() override;
	Coordinate getCoord();
	unsigned int getColor() override;

};
