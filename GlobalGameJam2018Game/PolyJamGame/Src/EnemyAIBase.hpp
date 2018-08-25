#pragma once

#include <functional>

#include <ECS/Scene.hpp>
#include <Collections/OrderedMap.hpp>

using namespace Poly;

namespace GGJGame
{
	class EnemyAIBase : public BaseObject<>
	{
	public:
		using actionSignature = std::function<bool(Scene* world, Entity* selfEntity)>;

		EnemyAIBase() : actions(OrderedMap<int, actionSignature>())
		{ }

		void AddAction(int priority, actionSignature action)
		{
			actions.Insert(priority, action);
		}

		void IterateActions(Scene* world, Entity* selfEntity)
		{
			for(auto action : actions)
			{
				// if action has been executed
				if(action.value(world, selfEntity))
					break;
			}

			RecreateActionsScript();
		}

	private:
		virtual void InitActions() = 0;

		void RecreateActionsScript()
		{
			actions.Clear();

			InitActions();
		}
		OrderedMap<int, actionSignature> actions;
	};

	class EnemyAIEngineer : public EnemyAIBase
	{
	public:
		EnemyAIEngineer()
		{
			InitActions();
		}
	private:
		virtual void InitActions() override final;
	};

	class EnemyAIAssailant : public EnemyAIBase
	{
	public:
		EnemyAIAssailant()
		{
			InitActions();
		}
	private:
		virtual void InitActions() override final;
	};
}