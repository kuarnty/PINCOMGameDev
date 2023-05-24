#pragma once

#include "Point2D.h"
#include "RenderingSource.h"
#include "Component.h"

namespace Game
{
	class Object
	{
	public:
		// Transform related
		Game::Point2D position;
		double rotation;
		double scale;

		// Rendering related
		Game::RenderingSource renderingSource;

		int numberOfComponents;
		Game::Component** componentList;

		Game::Component* getComponent(char* componentName);
		void update();
		void render();
	};
}