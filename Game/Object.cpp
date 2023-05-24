#include <string.h>

#include "Object.h"

Game::Component* Game::Object::getComponent(char* componentName)
{
	for (int i = 0; i < numberOfComponents; i++)
	{
		if (strcmp(componentList[i]->name, componentName) == 0)
			return componentList[i];
	}
	return NULL;
}

void Game::Object::update()
{
	for (int i = 0; i < numberOfComponents; i++)
	{
		componentList[i]->update();
	}
}

void Game::Object::render()
{

}