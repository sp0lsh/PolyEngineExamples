#pragma once

#include <ECS/ComponentBase.hpp>
#include <EnemyAIBase.hpp>

using namespace Poly;

namespace GGJGame
{
	class EnemyComponent : public ComponentBase
	{
		RTTI_DECLARE_COMPONENT(EnemyComponent) { NO_RTTI_PROPERTY(); }
	public:
		EnemyComponent(std::unique_ptr<EnemyAIBase> enemyAI) : EnemyAI(std::move(enemyAI))
		{ }

		std::unique_ptr<EnemyAIBase> EnemyAI;
		const float lineOfSight = 20.0f;
		float TransmitterDestroySpeed = 0.5f;
	};
}
