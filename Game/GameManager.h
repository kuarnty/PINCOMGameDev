#pragma once

#include <windows.h>

#include "Object.h"

namespace Game
{
	static class GameManager
	{
	public:
		static int numberOfObjects;
		static Game::Object** objectList;

		static void update();
		static void manageCollision();
		static void render();
		static void eventHandle();
		static void loop();
		static void init();
		static void winMsgHandle(MSG msg);
	};
}