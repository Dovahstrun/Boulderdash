#include "../Headers/Boulder.h"
#include "../../Framework/Headers/AssetManager.h"

Boulder::Boulder()
	: GridObject()
{
	m_sprite.setTexture(AssetManager::GetTexture("resources/graphics/boulder.png"));
}
