#include "PlayerControllerComponent.hpp"

namespace SGJ
{
	RTTI_DEFINE_COMPONENT(PlayerControllerComponent);

	PlayerControllerComponent::PlayerControllerComponent(float movementSpeed, float jumpForce) :
		DefJumpForce(jumpForce),
		MovementSpeed(movementSpeed),
		JumpForce(jumpForce)
	{
	}
}
