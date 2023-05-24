#pragma once

namespace Game
{
	class Object;

	class Component
	{
	public:
		char* name;
		Object* pParentObject;

		virtual void update() = 0;
	};
}