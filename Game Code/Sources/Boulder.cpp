#include "../Headers/Boulder.h"
#include "../../Framework/Headers/AssetManager.h"
#include "../Headers/Level.h"
#include "../Headers/Player.h"

Boulder::Boulder()
	: GridObject()
	, timer(0)
	, fallTime(0.3)
{
	m_sprite.setTexture(AssetManager::GetTexture("resources/graphics/boulder.png"));
	m_blocksMovement = true;
}

void Boulder::Update(sf::Time _frameTime)
{
	timer += _frameTime.asSeconds();
	if (timer > fallTime)
	{
		AttemptMove(sf::Vector2i(0, 1));
		timer = 0;
	}
}


bool Boulder::AttemptMove(sf::Vector2i _direction)
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

		//If so, attempt to dig (the blocker is dirt, not nullptr)
		if (landBoulder != nullptr)
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
				//m_footstep.play();
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
					//m_footstep.play();
					return m_level->MoveObjectTo(this, newTargetPos);
				}
			}
		}
	}

	//If movement is blocked, do nothing, return false
	//m_bump.play();
	return false;
}