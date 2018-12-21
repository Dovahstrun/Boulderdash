#pragma once

#include "GridObject.h"
#include "../../Framework/Headers/AnimationSystem.h"

class Exit : public GridObject
{

public:

	Exit();

	void setExit();
	bool getExit();

private:

	AnimationSystem m_animationSystem;
	Animation& open;
	bool m_exitOpen;

};