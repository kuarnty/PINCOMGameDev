#pragma once

namespace Game
{
	struct Resource
	{

	};

	struct ResourceRequest
	{

	};

	static class GraphicResourceManager
	{
	public:
		static Resource getResource(ResourceRequest request);
		static void init();
	};
}