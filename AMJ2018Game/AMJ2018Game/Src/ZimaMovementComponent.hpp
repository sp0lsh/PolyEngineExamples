#pragma once

#include <ECS/ComponentBase.hpp>

class GAME_DLLEXPORT ZimaMovementComponent : public Poly::ComponentBase
{
public:
	ZimaMovementComponent() = default;
	float GetSpeed() const { return Speed; }
	float GetDrag() const { return Drag; }
	Poly::Vector GetMoveVector() const { return MoveVector; }

	void SetSpeed(float speed) { Speed = speed; }


private:
	float Speed;
	float Drag;
	Poly::Vector MoveVector;

};