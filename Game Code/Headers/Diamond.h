#pragma once

#include "GridObject.h"

class Diamond : public GridObject
{

public:

	Diamond();

	void Update(sf::Time _frameTime);

	bool canItFall(sf::Vector2i _direction);
	bool AttemptFall(sf::Vector2i _direcion);
	bool AttemptSlide(sf::Vector2i _direction);

private: //Private Functions



private: //Private Variables

	float m_timer;
	float m_fallTime;
	bool m_hasFallen;

};