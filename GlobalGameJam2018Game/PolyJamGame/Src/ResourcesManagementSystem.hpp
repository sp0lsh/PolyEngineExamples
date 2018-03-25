#pragma once

#include <World.hpp>

using namespace Poly;

namespace GGJGame
{
	namespace ResourcesManagementSystem
	{
		void PreloadResources(World* world);
		void FreeResources(World* world);
	}
}