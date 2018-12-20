#pragma once

#include "GridObject.h"

class Boulder : public GridObject
{

public: //Public Functions

	Boulder();

	void Update(sf::Time _frameTime);

	bool AttemptFall(sf::Vector2i _direcion);
	bool AttemptPush(sf::Vector2i _direction);
	

private: //Private Functions

	

private: //Private Variables

	float m_timer;
	float m_fallTime;
	bool m_hasFallen;

};