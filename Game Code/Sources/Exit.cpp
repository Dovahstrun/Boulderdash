#include "../Headers/Exit.h"
#include "../../Framework/Headers/AssetManager.h"


Exit::Exit()
	: GridObject()
	, m_animationSystem()
	, m_exitOpen(false)
	, open(m_animationSystem.CreateAnimation("open"))
	, m_openSound()
{
	m_sprite.setTexture(AssetManager::GetTexture("resources/graphics/door1.png"));
	m_blocksMovement = true;

	///Create door opening animation
	//Set up the animation
	m_animationSystem.SetSprite(m_sprite);

	//Add the door frames
	open.AddFrame(AssetManager::GetTexture("resources/graphics/door1.png"));
	open.AddFrame(AssetManager::GetTexture("resources/graphics/door2.png"));
	open.AddFrame(AssetManager::GetTexture("resources/graphics/door3.png"));
	open.AddFrame(AssetManager::GetTexture("resources/graphics/door4.png"));

	//Set the playback speed, and set it to not loop
	open.SetPlayBackSpeed(6);
	open.SetLoop(false);

	///Set up sound effects
	m_openSound.setBuffer(AssetManager::GetSoundBuffer("resources/audio/door.wav"));
}

void Exit::Update(sf::Time _frameTime)
{
	//Process animations
	m_animationSystem.Update(_frameTime);
}

void Exit::setExit()
{
	m_exitOpen = true;
	m_animationSystem.Play("open");
	m_openSound.play();
}

bool Exit::getExit()
{
	return m_exitOpen;
}
