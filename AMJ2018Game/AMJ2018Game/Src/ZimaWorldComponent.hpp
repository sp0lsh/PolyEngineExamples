#pragma once

#include <UniqueID.hpp>
#include <Collections/Dynarray.hpp>
#include <ECS/ComponentBase.hpp>
#include <Rendering/MeshRenderingComponent.hpp>
#include <Rendering/PostprocessSettingsComponent.hpp>
#include <Rendering/Lighting/LightSourceComponent.hpp>

using namespace Poly;

class GAME_DLLEXPORT ZimaWorldComponent : public ComponentBase
{
public:

	SafePtr<Entity> Player;
	float PlayerHealth = 300.f;
	Dynarray<SafePtr<Entity>> Enemies;
	Dynarray<SafePtr<Entity>> Entities;
	Dynarray<SafePtr<Entity>> Bullets;

};