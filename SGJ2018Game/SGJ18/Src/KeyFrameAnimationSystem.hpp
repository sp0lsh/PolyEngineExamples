#pragma once

#include <AnimKeys.hpp>
#include <Math/Vector.hpp>
#include <Math/Quaternion.hpp>
#include <Collections/String.hpp>

namespace Poly
{
	class Scene;

	namespace KeyFrameAnimationSystem
	{
		void KeyFrameAnimationUpdatePhase(Scene* scene);

		OrderedMap<String, AnimKeys> LoadAnimTracks(String path);
		Vector AnimTrack_ReadVector3FromRow(String row);
		Quaternion AnimTrack_ReadQuternion4FromRow(String row);
	}
}