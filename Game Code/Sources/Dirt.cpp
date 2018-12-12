#include "../Headers/Dirt.h"
#include "../../Framework/Headers/AssetManager.h"

Dirt::Dirt()
	: GridObject()
{
	m_sprite.setTexture(AssetManager::GetTexture("resources/graphics/Dirt.png"));
	m_blocksMovement = true;
}