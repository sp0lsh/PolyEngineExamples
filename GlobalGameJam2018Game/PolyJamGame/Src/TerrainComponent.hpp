#pragma once

#include <ECS/ComponentBase.hpp>

using namespace Poly;

namespace GGJGame
{
	enum class eTerrainPassability
	{
		PASSABLE,
		IMPASSABLE,
		WALKABLE_GROUND,
		GROUND,
		_COUNT
	};

	class GAME_DLLEXPORT TerrainComponent : public ComponentBase
	{
		RTTI_DECLARE_COMPONENT(TerrainComponent) { NO_RTTI_PROPERTY(); }
	public:
		TerrainComponent(eTerrainPassability passability) : Passability(passability) {}

		eTerrainPassability Passability;
	};
}