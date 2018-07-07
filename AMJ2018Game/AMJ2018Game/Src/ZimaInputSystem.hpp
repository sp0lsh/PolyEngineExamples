#pragma once

#include <ECS/World.hpp>
#include <Collections/String.hpp>
#include <Math/Vector.hpp>
#include <Rendering/Particles/ParticleComponent.hpp>

using namespace Poly;

namespace ZimaSystem
{
	void Init(World* world);


	Entity* CreateActor(World* world, String path);


	void Update(World* world);
	void Deinit(World* world);
}

#pragma once

#include <ECS/World.hpp>
#include <Collections/String.hpp>
#include <Math/Vector.hpp>
#include <Rendering/Particles/ParticleComponent.hpp>

using namespace Poly;

namespace ZimaInputSystem
{
	void Update(World* world);
}
