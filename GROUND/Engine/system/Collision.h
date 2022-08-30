#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/intersect.hpp>
#include "../ECS.h"
#include "../Components/TransformComponent.h"

namespace gr {
    class Ray;

    class Collision
    {
    public:
        static bool AABB(Entity* a, Entity* b);
        static bool AABB(TransformComponent* one, TransformComponent* two);
        static bool AABB(TransformComponent* one, Entity* b);
        static bool AABB(Ray* r, Entity* e);
    };
}