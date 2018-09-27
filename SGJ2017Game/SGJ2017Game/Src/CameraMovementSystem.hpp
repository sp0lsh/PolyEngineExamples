#pragma once

#include <Defines.hpp>
#include <ECS/Scene.hpp>

namespace Poly { class Scene; }

namespace SGJ
{
	namespace CameraMovementSystem
	{
		void CameraMovementUpdatePhase(Poly::Scene* world);

		float ElasticEaseOut(float p);
	}
}