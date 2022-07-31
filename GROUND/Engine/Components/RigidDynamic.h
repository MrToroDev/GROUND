#pragma once

#include "../ECS.h"
#include "TransformComponent.h"

#include <PxPhysicsAPI.h>

namespace gr
{
	class RigidDynamic : public Component
	{
	public:
		RigidDynamic(physx::PxScene*& mScene, physx::PxPhysics*& mPhysics, physx::PxVec3 velocity, TransformComponent* transform);
		~RigidDynamic();

		void init() override;
		void update(float deltatime) override;

		physx::PxRigidDynamic* GetDynamic();

	private:
		TransformComponent* transform;
		physx::PxRigidDynamic* body;
	};
}