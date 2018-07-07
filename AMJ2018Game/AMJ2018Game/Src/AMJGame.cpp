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
	InitCommon();

	gDebugConfig.DisplayFPS = false;
	DeferredTaskSystem::AddWorldComponentImmediate<GameManagerWorldComponent>(gEngine->GetWorld());	

	bool bGameCamera = true;
	if (bGameCamera)
	{
		gConsole.LogInfo("AMJGame::Init register update phases");
		gEngine->RegisterGameUpdatePhase(ZimaInputSystem::Update);
		gEngine->RegisterGameUpdatePhase(ZimaSystem::Update);
		gEngine->RegisterGameUpdatePhase(ZimaGunSystem::Update);
		gEngine->RegisterGameUpdatePhase(ZimaBulletSystem::Update);
		gEngine->RegisterGameUpdatePhase(ZimaEnemySystem::Update);

		ZimaSystem::CreateCamera(gEngine->GetWorld());
		ZimaSystem::Init(gEngine->GetWorld());
	}
	else
	{
		gEngine->RegisterGameUpdatePhase(GameManagerSystem::Update);

		GameManagerSystem::CreateStartScene(gEngine->GetWorld());
	}
}
void AMJGame::InitCommon()
{
	gConsole.LogInfo("AMJGame::InitCommon");
	DeferredTaskSystem::AddWorldComponentImmediate<SkyboxWorldComponent>(gEngine->GetWorld(), "HDR/HDR.hdr", eResourceSource::GAME);
};

void AMJGame::Deinit()
{
	GameManagerSystem::Deinit(gEngine->GetWorld());
	ZimaSystem::Deinit(gEngine->GetWorld());
};