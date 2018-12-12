#include "../Headers/Exit.h"
#include "../../Framework/Headers/AssetManager.h"

Exit::Exit()
	: GridObject()
{
	m_sprite.setTexture(AssetManager::GetTexture("resources/graphics/Exit.png"));
}
