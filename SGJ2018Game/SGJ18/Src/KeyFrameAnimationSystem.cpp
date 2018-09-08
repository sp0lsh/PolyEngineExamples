#include "Game.hpp"
#include <KeyFrameAnimationSystem.hpp>
#include <AnimKeys.hpp>
#include <Collections/String.hpp>
#include <GameManagerWorldComponent.hpp>
#include <Time/TimeWorldComponent.hpp>
#include <KeyFrameAnimationComponent.hpp>
#include <KeyFrameCameraComponent.hpp>

using namespace Poly;

RTTI_DEFINE_COMPONENT(KeyFrameCameraComponent)
RTTI_DEFINE_COMPONENT(KeyFrameAnimationComponent)

void KeyFrameAnimationSystem::KeyFrameAnimationUpdatePhase(Scene* scene)
{

	GameManagerWorldComponent* gameMgrCmp = scene->GetWorldComponent<GameManagerWorldComponent>();
	//TODO implement
	if (scene->GetWorldComponent<InputWorldComponent>()->IsClicked(eKey::KEY_R))
	{
		gConsole.LogDebug("Reloading animations");
		gameMgrCmp->CameraAnimations = LoadAnimTracks("Animations/Camera.x");
	}

	float time = (float)(scene->GetWorldComponent<TimeWorldComponent>()->GetGameplayTime());
	float delta = (float)(TimeSystem::GetTimerDeltaTime(scene, Poly::eEngineTimer::GAMEPLAY));
	// const float currentFPS = 1.0f / delta;

	OrderedMap<String, AnimKeys>& cameraAnimations = gameMgrCmp->CameraAnimations;

	//update meshes transform
	for (auto [kfac, mrc] : scene->IterateComponents<KeyFrameAnimationComponent, MeshRenderingComponent>())
	{
		ASSERTE(mrc->GetMesh()->GetAnimations().GetSize() > 0, "No animations in MeshResource");
		MeshResource::Animation* anim = mrc->GetMesh()->GetAnimations()[0];

		const float targetFPS = (float)anim->TicksPerSecond;
		const MeshResource::Animation::Channel& channel = anim->channels[0];

		EnumArray<size_t, eAnimationKeyType> animKeySize;
		EnumArray<float, eAnimationKeyType> animTime;
		EnumArray<float, eAnimationKeyType> keyDelta;
		EnumArray<size_t, eAnimationKeyType> keyPrev;
		EnumArray<size_t, eAnimationKeyType> keyNext;

		animKeySize[eAnimationKeyType::POSITION] = channel.Positions.GetSize();
		animKeySize[eAnimationKeyType::SCALE] = channel.Scales.GetSize();
		animKeySize[eAnimationKeyType::ROTATION] = channel.Rotations.GetSize();

		for (eAnimationKeyType type : IterateEnum<eAnimationKeyType>())
		{
			animTime[type] = kfac->AnimProgress[type];
			animTime[type] += delta * targetFPS;
			animTime[type] = fmod(animTime[type], (float)animKeySize[type]);

			// keyDelta[type] = animTime[type] - floorf(animTime[type]); // fract actually but simpler
			keyDelta[type] = Clamp(animTime[type] - floorf(animTime[type]), 0.0f, 1.0f); // fract actually but simpler
			keyPrev[type] = (size_t)floorf(animTime[type]);
			keyNext[type] = keyPrev[type] + 1 >= animKeySize[type] ? 0 : keyPrev[type] + 1;
		}

		Quaternion rotationPrev = anim->channels[0].Rotations[keyPrev[eAnimationKeyType::ROTATION]].Value;
		Quaternion rotationNext = anim->channels[0].Rotations[keyNext[eAnimationKeyType::ROTATION]].Value;

		// gConsole.LogDebug("KeyFrameAnimationUpdatePhase keyPrev: {}, keyNext: {}, rotPrev: {}, rotNext: {}",
		// 	keyPrev[eAnimationKeyType::ROTATION], keyNext[eAnimationKeyType::ROTATION], rotationPrev.ToEulerAngles(), rotationNext.ToEulerAngles());

		Vector position = Lerp(channel.Positions[keyPrev[eAnimationKeyType::POSITION]].Value, channel.Positions[keyNext[eAnimationKeyType::POSITION]].Value, keyDelta[eAnimationKeyType::POSITION]);
		Vector scale = Lerp(channel.Scales[keyPrev[eAnimationKeyType::SCALE]].Value, channel.Scales[keyNext[eAnimationKeyType::SCALE]].Value, keyDelta[eAnimationKeyType::SCALE]);
		Quaternion rotation = Quaternion::Slerp(channel.Rotations[keyPrev[eAnimationKeyType::ROTATION]].Value, channel.Rotations[keyNext[eAnimationKeyType::ROTATION]].Value, keyDelta[eAnimationKeyType::ROTATION]);

		// gConsole.LogDebug("KeyFrameAnimationUpdatePhase pos, {}, rot: {}, scale: {}", position, rotation, scale);

		kfac->GetTransform().SetGlobalTranslation(position);
		kfac->GetTransform().SetGlobalScale(scale);
		kfac->GetTransform().SetGlobalRotation(rotation);

		//FIXME skew buildup haxx
		Matrix mtx = kfac->GetTransform().GetParentFromModel();
		Vector tra;
		Quaternion rot;
		Vector sca;
		MatrixSkew skew;
		Vector perspective;
		mtx.Decompose(tra, rot, sca, skew, perspective);
		kfac->GetTransform().SetLocalTranslation(tra);
		kfac->GetTransform().SetLocalRotation(rot);
		kfac->GetTransform().SetLocalScale(sca);

		//gConsole.LogDebug("org rot: {}", rotation);
		//gConsole.LogDebug("glb rot: {}", kfac->GetTransform().GetGlobalRotation());
		//gConsole.LogDebug("loc rot: {}", kfac->GetTransform().GetLocalRotation());

		kfac->AnimProgress[eAnimationKeyType::POSITION] = animTime[eAnimationKeyType::POSITION];
		kfac->AnimProgress[eAnimationKeyType::SCALE] = animTime[eAnimationKeyType::SCALE];
		kfac->AnimProgress[eAnimationKeyType::ROTATION] = animTime[eAnimationKeyType::ROTATION];
	}

	//update cameras postprocesses
	/*for (auto [kfcc, psc] : scene->IterateComponents<KeyFrameCameraComponent, PostprocessSettingsComponent>())
	{
		float animTime = kfcc->AnimProgress;
		AnimKeys& animKeys = cameraAnimations.Get(kfcc->Name).Value();
		size_t animKeysSize = animKeys.Positions.GetSize();
		float targetFPS = (float)animKeys.TicksPerSecond;

		animTime += delta;
		animTime = fmod(animTime, (float)animKeysSize);

		float keyDelta = animTime - floorf(animTime); // fract actually but simpler
		size_t keyPrev = (size_t)floorf(animTime);
		size_t keyNext = keyPrev + 1 >= animKeysSize ? 0 : keyPrev + 1;

		float keyDeltaSmooth = SmoothStep(0.0f, 1.0f, keyDelta);

		Vector position = Lerp(animKeys.Positions[keyPrev], animKeys.Positions[keyNext], keyDeltaSmooth);
		psc->GrainScale = position.X;
		psc->VignetteScale = position.Y;
		psc->AbberationScale = position.Z;

		Vector scale = Lerp(animKeys.Scales[keyPrev], animKeys.Scales[keyNext], keyDeltaSmooth);
		psc->MotionBlurScale = scale.X;
		psc->BloomThreshold = scale.Y;
		psc->BloomScale = scale.Z;

		Quaternion rotation = Quaternion::Slerp(animKeys.Rotations[keyPrev], animKeys.Rotations[keyNext].ToEulerAngles(), keyDeltaSmooth);
		psc->DOFPoint = rotation.X;
		psc->DOFRange = rotation.Y;
		psc->DOFSize = rotation.Z;
		//psc->DOFShow = rotation.W; //bug?

		kfcc->AnimProgress = animTime;
	}

	//update selected camera
	float animTime = gameMgrCmp->CameraSwitcherAnimProgress;
	AnimKeys& animKeys = gameMgrCmp->CameraSwitches;
	size_t animKeysSize = animKeys.Positions.GetSize();
	float targetFPS = (float)animKeys.TicksPerSecond;

	animTime += delta;
	animTime = fmod(animTime, (float)animKeysSize);

	size_t keyPrev = (size_t)floorf(animTime);

	float cameraId = animKeys.Positions[keyPrev].X;
	//gameMgrCmp->CurrentCameraIndex = static_cast <int> (std::floor(cameraId));

	gameMgrCmp->CameraSwitcherAnimProgress = animTime;*/

}

OrderedMap<String, AnimKeys> KeyFrameAnimationSystem::LoadAnimTracks(String path)
{
	gConsole.LogInfo("GameManagerSystem::LoadAnimTrack");
	String animSrc = LoadTextFileRelative(eResourceSource::GAME, path);
	// gConsole.LogInfo("GameManagerSystem::CreateAnimTrack AnimSrc: {}", animSrc);

	OrderedMap<String, AnimKeys> keys;

	String name;
	int animTicksPerSecond = 0;
	Dynarray<Vector> positions;
	Dynarray<Vector> scales;
	Dynarray<Quaternion> rotations;

	bool hasAnimTicksPerSecond = false;
	bool hasAnimationKeyRotation = false;
	bool hasAnimationKeyScale = false;
	bool hasAnimationKeyPosition = false;
	int rowCounterAnimTicksPerSecond = 0;
	int rowCounterRotation = 0;
	int rowCounterScale = 0;
	int rowCounterPosition = 0;

	animSrc.Replace("\r\n", "\n");
	animSrc.GetTrimmed();
	Dynarray<String> rows = animSrc.Split('\n');
	for (String row : rows)
	{
		if (positions.GetSize() > 0 && scales.GetSize() > 0 && rotations.GetSize() > 0 && !hasAnimationKeyPosition && !hasAnimationKeyRotation && !hasAnimationKeyScale)
		{
			gConsole.LogDebug("Saving transform animation for mesh {}", name);

			if (animTicksPerSecond == 0)
			{
				animTicksPerSecond = 20;
			}

			AnimKeys animKeys = AnimKeys(positions, rotations, scales, animTicksPerSecond);
			keys.Insert(name, animKeys);
			positions.Clear();
			rotations.Clear();
			scales.Clear();
			hasAnimationKeyRotation = false;
			hasAnimationKeyScale = false;
			hasAnimationKeyPosition = false;
			rowCounterRotation = 0;
			rowCounterScale = 0;
			rowCounterPosition = 0;
			continue;
		}
		if (row.Contains("{") && row.Contains("}"))
		{
			size_t startIdx = row.Find('{');
			size_t endIdx = row.Find('}');
			name = row.Substring(startIdx + 1, endIdx);
			continue;
		}
		if (row.IsEmpty())
		{
			continue;
		}

		if (row.Contains("AnimTicksPerSecond") && !row.Contains("template") && !row.Contains("DWORD"))
		{
			hasAnimTicksPerSecond = true;
		}

		if (row.Contains("Rotation"))
		{
			hasAnimationKeyRotation = true;
		}

		if (row.Contains("Scale"))
		{
			hasAnimationKeyScale = true;
		}

		if (row.Contains("Position"))
		{
			hasAnimationKeyPosition = true;
		}

		if (row.Contains("}"))
		{
			hasAnimationKeyRotation = false;
			hasAnimationKeyScale = false;
			hasAnimationKeyPosition = false;
			hasAnimTicksPerSecond = false;
		}

		if (hasAnimTicksPerSecond)
		{
			if (rowCounterAnimTicksPerSecond == 1)
			{
				String trimmed = row.GetTrimmed();
				animTicksPerSecond = (int)std::atoi(trimmed.Substring(0, trimmed.GetLength()).GetCStr());
			}

			rowCounterAnimTicksPerSecond++;
		}

		if (hasAnimationKeyRotation)
		{
			if (rowCounterRotation == 1)
			{
				gConsole.LogInfo("GameManagerSystem::CreateAnimTrack TODO: read if rotation or track ID?, row: {}", row);
			}

			if (rowCounterRotation == 2)
			{
				gConsole.LogInfo("GameManagerSystem::CreateAnimTrack TODO: number of keyframes, row: {}", row);
			}

			if (rowCounterRotation > 2)
			{
				rotations.PushBack(AnimTrack_ReadQuternion4FromRow(row));
			}

			rowCounterRotation++;
		}

		if (hasAnimationKeyScale)
		{
			if (rowCounterScale == 1)
			{
				gConsole.LogInfo("GameManagerSystem::CreateAnimTrack TODO: read if rotation or track ID?, row: {}", row);
			}

			if (rowCounterScale == 2)
			{
				gConsole.LogInfo("GameManagerSystem::CreateAnimTrack TODO: number of keyframes, row: {}", row);
			}

			if (rowCounterScale > 2)
			{
				scales.PushBack(AnimTrack_ReadVector3FromRow(row));
			}

			rowCounterScale++;
		}

		if (hasAnimationKeyPosition)
		{
			if (rowCounterPosition == 1)
			{
				gConsole.LogInfo("GameManagerSystem::CreateAnimTrack TODO: read if rotation or track ID?, row: {}", row);
			}

			if (rowCounterPosition == 2)
			{
				gConsole.LogInfo("GameManagerSystem::CreateAnimTrack TODO: number of keyframes, row: {}", row);
			}

			if (rowCounterPosition > 2)
			{
				positions.PushBack(AnimTrack_ReadVector3FromRow(row));
			}

			rowCounterPosition++;
		}
		
	}

	return keys;
}

Quaternion KeyFrameAnimationSystem::AnimTrack_ReadQuternion4FromRow(String row)
{
	// 0;4;0.000000, 0.000000, 0.000000, 0.000000;;,
	Dynarray<String> tokens = row.Split(';'); // values are at token with index 2 
	Dynarray<String> channels = tokens[2].Split(',');
	float w = (float)std::atof(channels[0].GetCStr());
	float x = (float)std::atof(channels[0].GetCStr());
	float y = (float)std::atof(channels[2].GetCStr());
	float z = (float)std::atof(channels[3].GetCStr());
	Quaternion q = Quaternion(x, y, z, w);

	//gConsole.LogDebug("Readed rotation: {}, euler {}", q, q.ToEulerAngles());
	return q;
}

Vector KeyFrameAnimationSystem::AnimTrack_ReadVector3FromRow(String row)
{
	// 0;4;0.000000, 0.000000, 0.000000, 0.000000;;,	
	Dynarray<String> tokens = row.Split(';'); // values are at token with index 2 
	Dynarray<String> channels = tokens[2].Split(',');
	float x = (float)std::atof(channels[0].GetCStr());
	float y = (float)std::atof(channels[0].GetCStr());
	float z = (float)std::atof(channels[2].GetCStr());
	Vector v = Vector(x, y, z);

	gConsole.LogDebug("Reading vector: {}", v);
	return v;
}