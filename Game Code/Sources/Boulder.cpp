#include "../Headers/Boulder.h"
#include "../../Framework/Headers/AssetManager.h"
#include "../Headers/Level.h"
#include "../Headers/Player.h"

Boulder::Boulder()
	: GridObject()
	, m_timer(0)
	, m_fallTime(0.3)
	, m_hasFallen(false)
{
	m_sprite.setTexture(AssetManager::GetTexture("resources/graphics/boulder.png"));
	m_blocksMovement = true;
}

void Boulder::Update(sf::Time _frameTime)
{
	///Boulders only fall periodically
	//Increase the m_timer
	m_timer += _frameTime.asSeconds();
	
	//If the m_timer is greater than the time between periods the boulders can fall...
	if (m_timer > m_fallTime)
	{
		//Attempt to move the boulder
		AttemptFall(sf::Vector2i(0, 1));

		//reset the m_timer
		m_timer = 0;
	}
}


bool Boulder::AttemptFall(sf::Vector2i _direction)
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
			///Need to check if either the cell to the left or right is blocked. Check the left first, and if the left is blocked, then check right
			///If either cell is unblocked, move there, and continue falling

			//Check if the cell to the right block movement
			//If not, move there
			sf::Vector2i newTargetPos = m_gridPosition;
			newTargetPos.x = m_gridPosition.x + 1;
			targetCellContents = m_level->getObjectAt(newTargetPos);
			blocked = false;
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
				return m_level->MoveObjectTo(this, newTargetPos);
			}
			else
			{
				//Check if the cell to the right block movement
				//If not, move there
				newTargetPos.x = m_gridPosition.x - 1;
				targetCellContents = m_level->getObjectAt(newTargetPos);
				blocked = false;
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
					return m_level->MoveObjectTo(this, newTargetPos);
				}
			}
		}
	}

	//If movement is blocked, it has not fallen, do nothing, return false
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
