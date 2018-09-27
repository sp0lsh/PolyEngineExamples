#pragma once

#include <ECS/Scene.hpp>

using namespace Poly;

namespace GGJGame
{
	namespace ResourcesManagementSystem
	{
		void PreloadResources(Scene* world);
		void FreeResources(Scene* world);
	}
}