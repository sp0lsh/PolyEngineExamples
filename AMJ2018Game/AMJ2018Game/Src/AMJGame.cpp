#include "stdlib.h" // srand rand

#include "AMJGame.hpp"
#include "GameManagerSystem.hpp"
#include "ZimaSystem.hpp"
#include "ZimaInputSystem.hpp"
#include "ZimaBulletSystem.hpp"
#include "ZimaGunSystem.hpp"
#include "ZimaEnemySystem.hpp"
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
	gEngine->RegisterGameUpdatePhase(ZimaInputSystem::Update);
	gEngine->RegisterGameUpdatePhase(ZimaSystem::Update);
	gEngine->RegisterGameUpdatePhase(ZimaGunSystem::Update);
	gEngine->RegisterGameUpdatePhase(ZimaBulletSystem::Update);
	gEngine->RegisterGameUpdatePhase(ZimaEnemySystem::Update);
	gEngine->RegisterGameUpdatePhase(GameManagerSystem::Update);

	DeferredTaskSystem::AddWorldComponentImmediate<GameManagerWorldComponent>(gEngine->GetWorld());

	ZimaSystem::Init(gEngine->GetWorld());

	gDebugConfig.DisplayFPS = false;

	bool bGameCamera = true;
	if (bGameCamera)
	{
		ZimaSystem::CreateCamera(gEngine->GetWorld());
	}
	else
	{
		GameManagerSystem::CreateStartScene(gEngine->GetWorld());
	}
};

void AMJGame::Deinit()
{
	GameManagerSystem::Deinit(gEngine->GetWorld());
};