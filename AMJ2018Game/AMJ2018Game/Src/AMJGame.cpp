#include "stdlib.h" // srand rand

#include "AMJGame.hpp"
#include "GameManagerSystem.hpp"
#include "ZimaSystem.hpp"
#include "ZimaInputSystem.hpp"
#include "GameManagerWorldComponent.hpp"

#include <Core.hpp>
#include <Audio/SoundSystem.hpp>
#include <ECS/DeferredTaskSystem.hpp>
#include <Rendering/MeshRenderingComponent.hpp>
#include <Rendering/PostprocessSettingsComponent.hpp>
#include <Rendering/ViewportWorldComponent.hpp>
#include <Rendering/Camera/CameraComponent.hpp>
#include <Rendering/Lighting/LightSourceComponent.hpp>
#include <Movement/FreeFloatMovementComponent.hpp>
#include <Resources/ResourceManager.hpp>
#include <Time/TimeWorldComponent.hpp>
#include <UI/ScreenSpaceTextComponent.hpp>

using namespace Poly;

DEFINE_GAME(AMJGame)

void AMJGame::Init()
{
	gEngine->RegisterGameUpdatePhase(GameManagerSystem::Update);
	gEngine->RegisterGameUpdatePhase(ZimaInputSystem::Update);

	DeferredTaskSystem::AddWorldComponentImmediate<GameManagerWorldComponent>(gEngine->GetWorld());

	GameManagerSystem::CreateScene(gEngine->GetWorld());
	ZimaSystem::Init(gEngine->GetWorld());

	bool bGameCamera = true;
	if (bGameCamera)
	{
		ZimaSystem::CreateCamera(gEngine->GetWorld());
	}
	else
	{
		GameManagerSystem::CreateCamera(gEngine->GetWorld());
	}
};

void AMJGame::Deinit()
{
	GameManagerSystem::Deinit(gEngine->GetWorld());
	ZimaSystem::Deinit(gEngine->GetWorld());
};