#include "ActorComponent.hpp"

using namespace GGJGame;

RTTI_DEFINE_COMPONENT(ActorComponent);

void GGJGame::ActorComponent::SetMovement(const Vector& movement)
{
	Movement = movement;
}

void GGJGame::ActorComponent::SetJumpRequested(bool val)
{
	if (!InAir || !val)
		JumpRequested = val;

	if (val)
		InAir = true;
}
