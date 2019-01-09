#include "../Headers/Diamond.h"
#include "../../Framework/Headers/AssetManager.h"
#include "../Headers/Level.h"
#include "../Headers/Player.h"

Diamond::Diamond()
	: GridObject()
	, m_timer(0)
	, m_fallTime(0.3)
	, m_hasFallen(false)
{
	m_sprite.setTexture(AssetManager::GetTexture("resources/graphics/diamond.png"));
	m_blocksMovement = true;
}

void Diamond::Update(sf::Time _frameTime)
{

	{
		///Diamonds only fall periodically and if they can fall
		//Increase the m_timer
		if (canItFall(sf::Vector2i(0, 1)))
		{
			m_timer += _frameTime.asSeconds();
		}
		else
		{
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
}


bool Diamond::canItFall(sf::Vector2i _direction)
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

		//If empty, increase the timer
		if (!blocked)
		{
			return true;
		}
		else
		{
			Player* playerToKill = dynamic_cast<Player*>(blocker);
			if (playerToKill != nullptr)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
}

bool Diamond::AttemptFall(sf::Vector2i _direction)
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
		//Check if the Diamond is now hitting the player
		Player* playerToKill = dynamic_cast<Player*>(blocker);
		if (playerToKill != nullptr)
		{
			m_level->ReloadLevel();
			return false;
		}

		//Do a dynamic cast to the Diamond to see if we can fall past it
		Diamond* landDiamond = dynamic_cast<Diamond*>(blocker);

		//If so, attempt to dig (the blocker is dirt, not nullptr)
		if (landDiamond != nullptr && m_hasFallen)
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

	//If movement is blocked, hasn't fallen, do nothing, return false
	m_hasFallen = false;
	return false;
}