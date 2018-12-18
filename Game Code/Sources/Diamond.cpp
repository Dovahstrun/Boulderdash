#include "../Headers/Diamond.h"
#include "../../Framework/Headers/AssetManager.h"

Diamond::Diamond()
	: GridObject()
{
	m_sprite.setTexture(AssetManager::GetTexture("resources/graphics/diamond.png"));
	m_blocksMovement = true;
}
