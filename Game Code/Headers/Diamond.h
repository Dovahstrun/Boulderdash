#pragma once

#include "GridObject.h"

class Diamond : public GridObject
{

public:

	Diamond();

	void Update(sf::Time _frameTime);

	bool AttemptMove(sf::Vector2i _direcion);

private: //Private Functions



private: //Private Variables

	float m_timer;
	float m_fallTime;
	bool m_hasFallen;

};