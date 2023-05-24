#pragma once

namespace Game
{
	enum Key
	{
		Up		= 00000001,
		Down	= 00000010,
		Left	= 00000100,
		Right	= 00001000,
		Esc		= 00010000,
		Enter	= 00100000,
	};

	static class Input
	{
	public:
		static int keyState;
		// 파일 날아가서 까먹음

		static void init();
		static void update();
	};
}