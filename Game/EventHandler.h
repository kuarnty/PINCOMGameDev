#pragma once

namespace Game
{
	static class EventHandler
	{
	public:
		static bool gameEnd;
		static bool escPressed;

		static void update();
	};
}