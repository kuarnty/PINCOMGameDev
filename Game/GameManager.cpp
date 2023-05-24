#include <typeinfo>

#include "GameManager.h"
#include "Collision.h"
#include "GraphicManager.h"
#include "EventHandler.h"
#include "Input.h"

void Game::GameManager::update()
{
	for (int i = 0; i < numberOfObjects; i++)
	{
		objectList[i]->update();
	}
}

void Game::GameManager::manageCollision()
{
	for (int i = 0; i < numberOfObjects; i++)
	{
		if (typeid(objectList[i]) == typeid(Game::Collision))
			// check collision
			// and run OnCollision(or else) of each obj.
			;
	}
}

void Game::GameManager::render()
{
	for (int i = 0; i < numberOfObjects; i++)
	{
		ResourceRequest rq;
		Game::GraphicManager::render(rq);
	}
}

void Game::GameManager::eventHandle() 
{
	if (EventHandler::gameEnd)
		; // game end
	else if (EventHandler::escPressed)
		; // stop game and show the settings
}

void Game::GameManager::loop()
{
	Game::Input::update();
	
	eventHandle();

	update();
	manageCollision();
	render();
}

void Game::GameManager::init()
{
	Game::GraphicResourceManager::init();
}

void Game::GameManager::winMsgHandle(MSG msg)
{

}