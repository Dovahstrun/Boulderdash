#pragma once

#include "GridObject.h"

class Boulder : public GridObject
{

public: //Public Functions

	Boulder();

	void Update(sf::Time _frameTime);

	bool AttemptMove(sf::Vector2i _direcion);

private: //Private Functions

	

private: //Private Variables

	float timer;
	float fallTime;

};