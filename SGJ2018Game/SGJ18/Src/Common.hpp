#pragma once

// Core
#include <Defines.hpp>
#include <RTTI/RTTI.hpp>

// Math
#include <Math/BasicMath.hpp>
#include <Math/Angle.hpp>
#include <Math/Vector.hpp>
#include <Math/Vector2f.hpp>
#include <Math/Vector2i.hpp>
#include <Math/Vector3f.hpp>
#include <Math/Matrix.hpp>
#include <Math/Quaternion.hpp>
#include <Math/SimdMath.hpp>
#include <Math/Random.hpp>
#include <Math/Frustum.hpp>

// Geometry
#include <Math/AABox.hpp>
#include <Math/AARect.hpp>

// Memory
#include <BaseObject.hpp>
#include <Memory/Allocator.hpp>
#include <Memory/PoolAllocator.hpp>
#include <Memory/IterablePoolAllocator.hpp>
#include <Memory/RefCountedBase.hpp>
#include <Memory/SafePtr.hpp>
#include <Memory/SafePtrRoot.hpp>

// Containers
#include <Collections/String.hpp>
#include <Collections/StringBuilder.hpp>
#include <Collections/Dynarray.hpp>
#include <Collections/Queue.hpp>
#include <Collections/PriorityQueue.hpp>
#include <Collections/OrderedMap.hpp>

// Other
#include <Math/Color.hpp>
#include <Utils/FileIO.hpp>
#include <Utils/Logger.hpp>
#include <UniqueID.hpp>
#include <Utils/EnumUtils.hpp>
#include <Utils/OutputStream.hpp>

#include <Movement/FreeFloatMovementComponent.hpp>
#include <ECS/DeferredTaskSystem.hpp>
#include <Rendering/Camera/CameraComponent.hpp>
#include <Time/TimeSystem.hpp>
#include <Rendering/ViewportWorldComponent.hpp>
#include <Rendering/MeshRenderingComponent.hpp>
#include <Rendering/PostprocessSettingsComponent.hpp>
#include <Input/InputWorldComponent.hpp>
#include <Resources/ResourceManager.hpp>