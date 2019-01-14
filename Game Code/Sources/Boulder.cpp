#include "../Headers/Boulder.h"
#include "../../Framework/Headers/AssetManager.h"
#include "../Headers/Level.h"
#include "../Headers/Player.h"
#include "../Headers/Diamond.h"

Boulder::Boulder()
	: GridObject()
	, m_timer(0)
	, m_fallTime(0.3)
	, m_hasFallen(false)
	, m_thudSound()
{
	m_sprite.setTexture(AssetManager::GetTexture("resources/graphics/boulder.png"));
	m_blocksMovement = true;

	///Sound effects
	m_thudSound.setBuffer(AssetManager::GetSoundBuffer("resources/audio/thud.wav"));
}

void Boulder::Update(sf::Time _frameTime)
{
	///Boulders only fall periodically and if they can fall
	//Increase the m_timer if the boulder can fall
	if (canItFall(sf::Vector2i(0,1)))
	{
		m_timer += _frameTime.asSeconds();
	}
	else
	{
		//If the boulder can't fall, reset the timer
		m_timer = 0;
	}
	
	//If the m_timer is greater than the time between periods the boulders can fall...
	if (m_timer > m_fallTime)
	{
		//Attempt to move the boulder
		AttemptFall(sf::Vector2i(0, 1));
		m_timer = 0;
	}
}


bool Boulder::canItFall(sf::Vector2i _direction)
{
	//Check if we can fall in the given direction

	//Get your current position
	//Calculate target position
	sf::Vector2i targetPos = m_gridPosition + _direction;

	//Check if the space is empty
	//Get list of objects in target position (targetpos)
	std::vector<GridObject*> targetCellContents = m_level->getObjectAt(targetPos);
	//Check if any of those objects block movement
	bool blocked = false;
	GridObject* blocker = nullptr;
	for (int i = 0; i < targetCellContents.size(); ++i)
	{
		if (targetCellContents[i]->getBlocksMovement() == true)
		{
			blocked = true;
			blocker = targetCellContents[i];
		}
	}

	//If empty, return true to increase the timer
	if (!blocked)
	{
		return true;
	}
	else
	{
		//If there is a boulder or a player beneath the boulder, we still want to be able to fall. If the boulder doesn't fall then it can't kill the player, and it also won't be able to slide
		Player* playerToKill = dynamic_cast<Player*>(blocker);
		Boulder* landBoulder = dynamic_cast<Boulder*>(blocker);
		Diamond* landDiamond = dynamic_cast<Diamond*>(blocker);
		if (playerToKill != nullptr)
		{
			return true;
		}
		else if (landBoulder != nullptr)
		{
			return true;
		}
		else if (landDiamond != nullptr)
		{
			return true;
		}
		else
		{
			//Play the thud sound if the boulder fell previously
			if (m_hasFallen)
			{
				m_hasFallen = false;
				m_thudSound.play();
			}
			return false;
		}
	}

}

bool Boulder::AttemptFall(sf::Vector2i _direction)
{
	//Attempt to move in the given direction

	//Get your current position
	//Calculate target position
	sf::Vector2i targetPos = m_gridPosition + _direction;

	//Check if the space is empty
	//Get list of objects in target position (targetpos)
	std::vector<GridObject*> targetCellContents = m_level->getObjectAt(targetPos);
	//Check if any of those objects block movement
	bool blocked = false;
	GridObject* blocker = nullptr;
	for (int i = 0; i < targetCellContents.size(); ++i)
	{
		if (targetCellContents[i]->getBlocksMovement() == true)
		{
			blocked = true;
			blocker = targetCellContents[i];
		}
	}

	//If empty move there

	if (!blocked)
	{
		m_hasFallen = true;
		return m_level->MoveObjectTo(this, targetPos);
	}
	else
	{
		//We were blocked!

		//Can we interact with the thing blocking us?
		//Check if the boulder is now hitting the player
		Player* playerToKill = dynamic_cast<Player*>(blocker);
		if (playerToKill != nullptr)
		{
			m_level->ReloadLevel();
			return false;
		}

		//Do a dynamic cast to the boulder to see if we can fall past it
		Boulder* landBoulder = dynamic_cast<Boulder*>(blocker);

		//If so, attempt to fall (the blocker is a boulder, not nullptr)
		if (landBoulder != nullptr && m_hasFallen)
		{
			//Play a thud sound as the boulder has landed
			m_thudSound.play();

			///Need to check if either the cell to the left or right is blocked. Check the left first, and if the left is blocked, then check right
			///If either cell is unblocked, move there, and continue falling

			if (!AttemptSlide(sf::Vector2i(1, 0)))
			{
				AttemptSlide(sf::Vector2i(-1, 0));
			}
		}

		Diamond* landDiamond = dynamic_cast<Diamond*>(blocker);

		//If so, attempt to fall (the blocker is a boulder, not nullptr)
		if (landDiamond != nullptr && m_hasFallen)
		{
			//Play a thud sound as the boulder has landed
			m_thudSound.play();

			///Need to check if either the cell to the left or right is blocked. Check the left first, and if the left is blocked, then check right
			///If either cell is unblocked, move there, and continue falling

			if (!AttemptSlide(sf::Vector2i(1, 0)))
			{
				AttemptSlide(sf::Vector2i(-1, 0));
			}
		}
	}

	//If movement is blocked, it has not fallen, play the thud sound, return false
	

	m_hasFallen = false;
	return false;
}

bool Boulder::AttemptPush(sf::Vector2i _direction)
{
	{
		//Attempt to move in the given direction

		//Get your current position
		//Calculate target position
		sf::Vector2i targetPos = m_gridPosition + _direction;

		//TODO: Check if the space is empty
		//Get list of objects in target position (targetpos)
		std::vector<GridObject*> targetCellContents = m_level->getObjectAt(targetPos);
		//Check if any of those objects block movement
		bool blocked = false;
		for (int i = 0; i < targetCellContents.size(); ++i)
		{
			if (targetCellContents[i]->getBlocksMovement() == true)
			{
				blocked = true;
			}
		}

		//If empty move there

		if (!blocked)
		{
			return m_level->MoveObjectTo(this, targetPos);
		}

		//If movement is blocked, do nothing, return false
		return false;
	}
}

bool Boulder::AttemptSlide(sf::Vector2i _direction)
{
	//Get your current position
	//Calculate target position
	sf::Vector2i targetPos = m_gridPosition + _direction;

	//Check if the space is empty
	//Get list of objects in target position (targetpos)
	std::vector<GridObject*> targetCellContents = m_level->getObjectAt(targetPos);
	//Check if any of those objects block movement
	bool blocked = false;
	//Check if any of those objects block movement
	for (int i = 0; i < targetCellContents.size(); ++i)
	{
		if (targetCellContents[i]->getBlocksMovement() == true)
		{
			blocked = true;
		}
	}

	//If the cell to the left is not blocked, move there
	if (!blocked)
	{
		m_hasFallen = false;
		return m_level->MoveObjectTo(this, targetPos);
	}

	return false;
}
