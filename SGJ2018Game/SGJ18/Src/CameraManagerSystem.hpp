#pragma once
#include "EnginePCH.hpp"

using namespace Poly;

namespace SGJJam
{
	namespace CameraManagerSystem
	{
		void SwitchCameraCyclic(Scene* world);
		void SwitchCameraTo(Scene* world, size_t ind);
		void Update(Scene* world);
	}
};