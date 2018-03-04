#include "stdlib.h" // srand rand

#include "RenderingSandbox.hpp"
#include "GameManagerSystem.hpp"
#include "GameManagerWorldComponent.hpp"

#include <Core.hpp>
#include <DeferredTaskSystem.hpp>
#include <CameraComponent.hpp>
#include <MeshRenderingComponent.hpp>
#include <FreeFloatMovementComponent.hpp>
#include <ScreenSpaceTextComponent.hpp>
#include <DeferredTaskSystem.hpp>
#include <ViewportWorldComponent.hpp>
#include <LightSourceComponent.hpp>
#include <PostprocessSettingsComponent.hpp>
#include <ResourceManager.hpp>
#include <TimeWorldComponent.hpp>
#include <SoundSystem.hpp>
#include <SoundResource.hpp>

using namespace Poly;

DEFINE_GAME(RenderingSandbox)

void RenderingSandbox::Init()
{
	gEngine->RegisterGameUpdatePhase(GameManagerSystem::Update);
	DeferredTaskSystem::AddWorldComponentImmediate<GameManagerWorldComponent>(gEngine->GetWorld());

	// audio https://www.newgrounds.com/audio/listen/772810
	SoundsPreload.PushBack(ResourceManager<SoundResource>::Load("March.ogg", eResourceSource::GAME));

	// https://earlybeatles.com/yellow.html
	SoundsPreload.PushBack(ResourceManager<SoundResource>::Load("Yellow_Submarine.ogg", eResourceSource::GAME));

	GameManagerSystem::CreateScene(gEngine->GetWorld());
};

void RenderingSandbox::Deinit()
{
	for (SoundResource* res : SoundsPreload)
		ResourceManager<SoundResource>::Release(res);

	GameManagerSystem::Deinit(gEngine->GetWorld());
};