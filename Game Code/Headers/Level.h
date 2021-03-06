#pragma once

//Library Includes
#include <SFML/Graphics.hpp>
#include "GridObject.h"

class Level
{

public:

	Level();

	virtual void Draw(sf::RenderTarget& _target);
	virtual void Update(sf::Time _frameTime);
	void Input(sf::Event _gameEvent);

	void loadLevel(int _levelToLoad);
	void ReloadLevel();
	void loadNextLevel();
	int GetCurrentLevel();

	int getCellSize();
	bool MoveObjectTo(GridObject* _toMove, sf::Vector2i _targetPos);
	std::vector<GridObject*> getObjectAt(sf::Vector2i _targetPos);
	void deleteObjectAt(GridObject* _toDelete);

	bool checkComplete();
	bool canExitOpen();

private:

	const float m_cellSize;
	int m_currentLevel;
	bool m_pendingReload;
	int m_pendingLevel;
	std::vector<std::vector<sf::Sprite> > m_background;
	std::vector<std::vector<std::vector<GridObject*> > > m_contents;

	sf::Sound m_backgroundMusic;
};
