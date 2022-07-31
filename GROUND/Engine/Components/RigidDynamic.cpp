#include "RigidDynamic.h"

gr::RigidDynamic::RigidDynamic(physx::PxScene*& mScene, physx::PxPhysics*& mPhysics, physx::PxVec3 velocity, TransformComponent* transform)
{
    physx::PxMaterial* mMaterial = mPhysics->createMaterial(0.5f, 0.5f, 0.6f);
    physx::PxTransform t = physx::PxTransform(physx::PxVec3(
        transform->position.x,
        transform->position.y,
        transform->position.z
    ));
    body = physx::PxCreateDynamic(*mPhysics, t, physx::PxBoxGeometry(transform->size.x, transform->size.y, transform->size.z), *mMaterial, 10.0f);
    body->setAngularDamping(0.5f);
    body->setLinearVelocity(velocity);
    mScene->addActor(*body);
}

gr::RigidDynamic::~RigidDynamic()
{
}

void gr::RigidDynamic::init()
{
    transform = &entity->getComponent<gr::TransformComponent>();
}

void gr::RigidDynamic::update(float deltatime)
{

    transform->position = glm::vec3(
        body->getGlobalPose().p.x,
        body->getGlobalPose().p.y,
        body->getGlobalPose().p.z
    );
    auto axis = physx::PxVec3(0.0);
    body->getGlobalPose().q.toRadiansAndUnitAxis(transform->angle, axis);
    transform->angleAxis = glm::vec3(axis.x, axis.y, axis.z);
}

physx::PxRigidDynamic* gr::RigidDynamic::GetDynamic()
{
    return body;
}
