#pragma once

#include <ECS/ComponentBase.hpp>

using namespace Poly;

class GAME_DLLEXPORT ZimaBulletComponent : public ComponentBase
{
public:
	float Speed = 300.f;
	float LifeTime = 0.f;
};
