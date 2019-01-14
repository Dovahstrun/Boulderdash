//Library Includes
#include <iostream>
#include <fstream>

//Project Includes

#include "../Headers/Level.h"
#include "../../Framework/Headers/AssetManager.h"
#include "../Headers/Wall.h"
#include "../Headers/Player.h"
#include "../Headers/Diamond.h"
#include "../Headers/Dirt.h"
#include "../Headers/Exit.h"
#include "../Headers/Boulder.h"

Level::Level()
	: m_cellSize(64.0f)
	, m_currentLevel(0)
	, m_pendingReload(false)
	, m_pendingLevel(0)
	, m_background()
	, m_contents()
	, m_backgroundMusic()
{
	loadLevel(1);
	//m_backgroundMusic.setBuffer(AssetManager::GetSoundBuffer("resources/audio/Boulderdash.wav"));
	m_backgroundMusic.setVolume(50);
	//m_backgroundMusic.play();
	m_backgroundMusic.setLoop(true);
}

void Level::Draw(sf::RenderTarget & _target)
{
	//Create and update camera
	sf::View camera = _target.getDefaultView();
	
	//Draw game world to the camera
	_target.setView(camera);
	for (int y = 0; y < m_background.size(); ++y)
	{
		for (int x = 0; x < m_background[y].size(); ++x)
		{
			_target.draw(m_background[y][x]);
		}
	}

	//Draw game objects
	for (int y = 0; y < m_contents.size(); ++y)//rows
	{
		for (int x = 0; x < m_contents[y].size(); ++x)//cells
		{
			for (int z = 0; z < m_contents[y][x].size(); ++z) //Sticky outies (grid objects)
			{
				m_contents[y][x][z]->Draw(_target);
			}
		}
	}


	// Draw UI to the window
	_target.setView(_target.getDefaultView());
	//Draw UI objects
	//TODO
}

void Level::Update(sf::Time _frameTime)
{
	// rows
	for (int y = 0; y < m_contents.size(); ++y)
	{
		// cells
		for (int x = 0; x < m_contents[y].size(); ++x)
		{
			// sticky outies (grid objects)
			for (int z = 0; z < m_contents[y][x].size(); ++z)
			{
				m_contents[y][x][z]->Update(_frameTime);
			}
		}
	}

	//If there is a pending reload waiting
	if (m_pendingReload)
	{
		//Reload level
		loadLevel(m_currentLevel);


		//Set pending reload to false
		m_pendingReload = false;
	}

	//If there is a pending level waiting

	if (m_pendingLevel != 0)
	{
		//Load it
		loadLevel(m_pendingLevel);

		m_pendingLevel = 0;
	}
}

void Level::Input(sf::Event _gameEvent)
{
	for (int y = 0; y < m_contents.size(); ++y)//rows
	{
		for (int x = 0; x < m_contents[y].size(); ++x)//cells
		{
			for (int z = 0; z < m_contents[y][x].size(); ++z) //Sticky outies (grid objects)
			{
				m_contents[y][x][z]->Input(_gameEvent);
			}
		}
	}

	//If backspace is pressed, reload level
	if (_gameEvent.type == sf::Event::KeyPressed)
	{
		// Yes it was a key press!

		// What key was pressed?
		if (_gameEvent.key.code == sf::Keyboard::BackSpace)
		{
			ReloadLevel();
		}
	}

	if (_gameEvent.type == sf::Event::KeyPressed)
	{
		// Yes it was a key press!

		// What key was pressed?
		if (_gameEvent.key.code == sf::Keyboard::Escape)
		{
			exit(0);
		}
	}

	if (_gameEvent.type == sf::Event::KeyPressed)
	{
		// Yes it was a key press!

		// What key was pressed?
		if (_gameEvent.key.code == sf::Keyboard::Enter)
		{
			loadNextLevel();
		}
	}
}

void Level::loadLevel(int _levelToLoad)
{

	///Cleanup the old level

	//Delete any data already in the level
	for (int y = 0; y < m_contents.size(); ++y)//rows
	{
		for (int x = 0; x < m_contents[y].size(); ++x)//cells
		{
			for (int z = 0; z < m_contents[y][x].size(); ++z) //Sticky outies (grid objects)
			{
				delete m_contents[y][x][z];
			}
		}
	}

	//Clear out the lists
	m_background.clear();
	m_contents.clear();

	///Setup everything

	//Set current level
	m_currentLevel = _levelToLoad;

	//Set up all the game objects
	//Open our file for reading
	std::ifstream inFile;
	std::string fileName = "levels/Level" + std::to_string(m_currentLevel) + ".txt";
	inFile.open(fileName);

	//Make sure the file was opened
	if (!inFile)
	{
		std::cerr << "Unable to open file " + fileName;
		exit(1); //Call system to stop program with error
	}

	//Set the starting x and y coords used to position level objects
	int x = 0;
	int y = 0;

	//Create the first row in our grid
	m_background.push_back(std::vector<sf::Sprite>());
	m_contents.push_back(std::vector<std::vector<GridObject*> >());

	//Reading each character one by one from the file...
	char ch;
	//Each time, try to read the next character, execute body of loop
	while (inFile >> std::noskipws >> ch)//the noskipws means we will include the white space (spaces)
	{
		//Perform actions based on what character was read in
		if (ch == ' ')
		{
			++x;
		}
		else if (ch == '\n')
		{
			++y;
			x = 0;

			//Create a new row in our grid
			m_background.push_back(std::vector<sf::Sprite>());
			m_contents.push_back(std::vector<std::vector<GridObject*> >());
		}
		else
		{
			//Create background sprite (this is going to be some object/empty space, so we need a background)
			m_background[y].push_back(sf::Sprite(AssetManager::GetTexture("resources/graphics/ground.png")));
			m_background[y][x].setPosition(x*m_cellSize, y*m_cellSize);

			//Create an empty vector for our grid contents in this cell
			m_contents[y].push_back(std::vector<GridObject*>());

			if (ch == '-')
			{
				Dirt* dirt = new Dirt();
				dirt->setLevel(this);
				dirt->setGridPosition(x, y);
				m_contents[y][x].push_back(dirt);
			}
			else if (ch == 'W')
			{
				Wall* wall = new Wall();
				wall->setLevel(this);
				wall->setGridPosition(x, y);
				m_contents[y][x].push_back(wall);
			}
			else if (ch == 'P')
			{
				Player* player = new Player();
				player->setLevel(this);
				player->setGridPosition(x, y);
				m_contents[y][x].push_back(player);
			}
			else if (ch == 'D')
			{
				Diamond* diamond = new Diamond();
				diamond->setLevel(this);
				diamond->setGridPosition(x, y);
				m_contents[y][x].push_back(diamond);
			}
			else if (ch == 'E')
			{
				Exit* exit = new Exit();
				exit->setLevel(this);
				exit->setGridPosition(x, y);
				m_contents[y][x].push_back(exit);
			}
			else if (ch == 'B')
			{
				Boulder* boulder = new Boulder();
				boulder->setLevel(this);
				boulder->setGridPosition(x, y);
				m_contents[y][x].push_back(boulder);
			}
			else
			{
				std::cerr << "Unrecognised character in level file: " << ch;
			}
		}
		
	}

	//Close the file now that we're done
	inFile.close();

	//Close the file now that we're done
	
}

void Level::ReloadLevel()
{
	m_pendingReload = true;
}

void Level::loadNextLevel()
{
	m_pendingLevel = m_currentLevel + 1;
}

int Level::GetCurrentLevel()
{
	return m_currentLevel;
}

int Level::getCellSize()
{
	return m_cellSize;
}

bool Level::MoveObjectTo(GridObject * _toMove, sf::Vector2i _targetPos)
{
	//Don't trust other code. Make sure _toMove is a valid pointer
	if (_toMove != nullptr 
		&& _targetPos.y >= 0 && _targetPos.y < m_contents.size() 
		&& _targetPos.x >= 0 && _targetPos.x < m_contents[_targetPos.y].size())
	{

		//Get the current position of our grid object
		sf::Vector2i Pos = _toMove->getGridPosition();

		//Find the object in the list using an iterator and the find method
		auto it = std::find(m_contents[Pos.y][Pos.x].begin(),
			m_contents[Pos.y][Pos.x].end(),
			_toMove);

		//If we found the object at this location, it will NOT equal the end of the vector
		if (it != m_contents[Pos.y][Pos.x].end())
		{
			//We found the object!

			//Remove it from the old position
			m_contents[Pos.y][Pos.x].erase(it);

			//Add it to its new position
			m_contents[_targetPos.y][_targetPos.x].push_back(_toMove);

			//Tell the object its new position
			
			_toMove->setGridPosition(_targetPos);

			//Return success
			return true;

		}
	}

	//return failure
	return false;
}

std::vector<GridObject*> Level::getObjectAt(sf::Vector2i _targetPos)
{
	//Don't trust the data
	//Make sure the co-ords are within the vector size
	if (_targetPos.y >= 0 && _targetPos.y < m_contents.size()
		&& _targetPos.x >= 0 && _targetPos.x < m_contents[_targetPos.y].size())
	{
		//Get the contents
		return m_contents[_targetPos.y][_targetPos.x];
	}

	//Default return
	return std::vector<GridObject*>(); //return an empty vector with nothing in it (default constructor)

}

void Level::deleteObjectAt(GridObject * _toDelete)
{
	//Don't trust other code. Make sure _toDelete is a valid pointer
	if (_toDelete != nullptr)
	{

		//Get the current position of our grid object
		sf::Vector2i Pos = _toDelete->getGridPosition();

		//Find the object in the list using an iterator and the find method
		auto it = std::find(m_contents[Pos.y][Pos.x].begin(),
			m_contents[Pos.y][Pos.x].end(),
			_toDelete);

		//If we found the object at this location, it will NOT equal the end of the vector
		if (it != m_contents[Pos.y][Pos.x].end())
		{
			//We found the object!

			//Delete the dirt
			delete *it;
			//Remove it from the old position
			m_contents[Pos.y][Pos.x].erase(it);

		}
	}

}

bool Level::checkComplete()
{
	for (int y = 0; y < m_contents.size(); ++y)
		//rows
	{
		for (int x = 0; x < m_contents[y].size(); ++x)//cells
		{
			for (int z = 0; z < m_contents[y][x].size(); ++z) //Sticky outies (grid objects)
			{
				GridObject* thisObject = m_contents[y][x][z];

				Exit* thisExit = dynamic_cast<Exit*>(thisObject);
				if (thisExit != nullptr && !thisExit->getExit())
				{
					return false;
				}
			}
		}
	}

	//The exit has been passed through

	//Create a pending level, as otherwise we will get an access violation as we are still in the middle of update
	loadNextLevel();

	//The level is complete, so return true
	return true;
}

bool Level::canExitOpen()
{

	{
		///Loop through an check all cells to see if there are any diamonds
		for (int y = 0; y < m_contents.size(); ++y)
			//rows
		{
			for (int x = 0; x < m_contents[y].size(); ++x)//cells
			{
				for (int z = 0; z < m_contents[y][x].size(); ++z) //Sticky outies (grid objects)
				{
					GridObject* thisObject = m_contents[y][x][z];

					Diamond* diamondObject = dynamic_cast<Diamond*>(thisObject);
					if (diamondObject != nullptr)
					{
						//It was a diamond
						return false;
					}
				}
			}
		}

		//All diamonds have been collected so player can pass through the exit

		///Loop through an check all cells to see if there are any diamonds
		for (int y = 0; y < m_contents.size(); ++y)
			//rows
		{
			for (int x = 0; x < m_contents[y].size(); ++x)//cells
			{
				for (int z = 0; z < m_contents[y][x].size(); ++z) //Sticky outies (grid objects)
				{
					GridObject* thisObject = m_contents[y][x][z];

					Exit* thisExit = dynamic_cast<Exit*>(thisObject);
					if (thisExit != nullptr)
					{
						thisExit->setExit();
					}
				}
			}
		}

		//The exit has opened so return true
		return true;
	}
}


