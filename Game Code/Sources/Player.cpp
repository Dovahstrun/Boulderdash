// Project Includes
#include "../Headers/Player.h"
#include "../../Framework/Headers/AssetManager.h"
#include "../Headers/Level.h"
#include "../Headers/Dirt.h"
#include "../Headers/Diamond.h"

Player::Player()
	: GridObject()
	, m_footstep()
	, m_dig()
	, m_bump()
	, m_gem()
{
	m_sprite.setTexture(AssetManager::GetTexture("resources/graphics/player/playerStandDown.png"));
	m_footstep.setBuffer(AssetManager::GetSoundBuffer("resources/audio/floor_step.wav"));
	m_dig.setBuffer(AssetManager::GetSoundBuffer("resources/audio/footstep1.ogg"));
	m_bump.setBuffer(AssetManager::GetSoundBuffer("resources/audio/bump.wav"));
	m_gem.setBuffer(AssetManager::GetSoundBuffer("resources/audio/ding.wav"));
}

void Player::Input(sf::Event _gameEvent)
{
	//read the input from the keyboard and convert it to a direction to move in

	//was the event a key press
	if (_gameEvent.type == sf::Event::KeyPressed)
	{
		// Yes it was a key press!

		// What key was pressed?
		if (_gameEvent.key.code == sf::Keyboard::W)
		{
			// It was W!
			// Move up
			m_pendingMove = sf::Vector2i(0, -1);
			m_sprite.setTexture(AssetManager::GetTexture("resources/graphics/player/playerStandUp.png"));
		}
		else if (_gameEvent.key.code == sf::Keyboard::A)
		{
			// It was A!
			// Move left
			m_pendingMove = sf::Vector2i(-1, 0);
			m_sprite.setTexture(AssetManager::GetTexture("resources/graphics/player/playerStandLeft.png"));
		}
		else if (_gameEvent.key.code == sf::Keyboard::S)
		{
			// It was S!
			// Move down
			m_pendingMove = sf::Vector2i(0, 1);
			m_sprite.setTexture(AssetManager::GetTexture("resources/graphics/player/playerStandDown.png"));
		}
		else if (_gameEvent.key.code == sf::Keyboard::D)
		{
			// It was D!
			// Move right
			m_pendingMove = sf::Vector2i(1, 0);
			m_sprite.setTexture(AssetManager::GetTexture("resources/graphics/player/playerStandRight.png"));
		}
	}
}

void Player::Update(sf::Time _frameTime)
{
	// If we have movement waiting to be processed,
	if (m_pendingMove.x != 0 || m_pendingMove.y != 0)
	{
		// move in that direction
		AttemptMove(m_pendingMove);

		// and clear the pending movement
		m_pendingMove = sf::Vector2i(0, 0);
	}
}


bool Player::AttemptMove(sf::Vector2i _direction)
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
		m_footstep.play();
		return m_level->MoveObjectTo(this, targetPos);
	}
	else
	{
		//We were blocked!

		//Can we interact with the thing blocking us?
		//Do a dynamic cast to dirt to see if we can dig it
		Dirt* dugDirt = dynamic_cast<Dirt*>(blocker);

		//If so, attempt to dig (the blocker is dirt, not nullptr)
		if (dugDirt != nullptr)
		{
			//Delete the dirt
			m_level->deleteObjectAt(dugDirt);
			
			//Play dig sound
			m_dig.play();

			//Move to new spot (where dirt was)
			return m_level->MoveObjectTo(this, targetPos);
		}

		//Can we interact with the thing blocking us?
		//Do a dynamic cast to dirt to see if we can dig it
		Diamond* diamond = dynamic_cast<Diamond*>(blocker);

		//If so, attempt to dig (the blocker is dirt, not nullptr)
		if (diamond != nullptr)
		{
			//Delete the diamond
			m_level->deleteObjectAt(diamond);

			//Play dig sound
			m_gem.play();

			//Move to new spot (where dirt was)
			return m_level->MoveObjectTo(this, targetPos);
		}

	}

	//If movement is blocked, do nothing, return false
	m_bump.play();
	return false;
}
