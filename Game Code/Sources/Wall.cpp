#include "../Headers/Wall.h"
#include "../../Framework/Headers/AssetManager.h"

Wall::Wall()
	: GridObject()
{
	m_sprite.setTexture(AssetManager::GetTexture("graphics/wall.png"));
	m_blocksMovement = true;
}
