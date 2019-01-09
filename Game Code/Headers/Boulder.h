#pragma once

#include "GridObject.h"

class Boulder : public GridObject
{

public: //Public Functions

	Boulder();

	void Update(sf::Time _frameTime);

	bool canItFall(sf::Vector2i _direction);
	bool AttemptFall(sf::Vector2i _direction);
	bool AttemptPush(sf::Vector2i _direction);

	void setTimer(float _time);
	

private: //Private Functions

	

private: //Private Variables

	float m_timer;
	float m_fallTime;
	bool m_hasFallen;

};