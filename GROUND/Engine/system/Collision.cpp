#include "Collision.h"
#include "../graphics/Ray.h"

namespace gr
{
    bool Collision::AABB(Entity* a, Entity* b)
    {
        TransformComponent* one = &a->getComponent<TransformComponent>();
        TransformComponent* two = &b->getComponent<TransformComponent>();

        bool collisionX = one->position.x + one->size.x >= two->position.x &&
            two->position.x + two->size.x >= one->position.x;
        // collision y-axis?
        bool collisionY = one->position.y + one->size.y >= two->position.y &&
            two->position.y + two->size.y >= one->position.y;
                
        bool collisionZ = one->position.z + one->size.z >= two->position.z &&
            two->position.z + two->size.z >= one->position.z;

        return collisionX && collisionY && collisionZ;
    }

    bool Collision::AABB(TransformComponent* one, TransformComponent* two)
    {
        bool collisionX = one->position.x + one->size.x >= two->position.x &&
            two->position.x + two->size.x >= one->position.x;
        // collision y-axis?
        bool collisionY = one->position.y + one->size.y >= two->position.y &&
            two->position.y + two->size.y >= one->position.y;
            
        bool collisionZ = one->position.z + one->size.z >= two->position.z &&
            two->position.z + two->size.z >= one->position.z;
        
        return collisionX && collisionY && collisionZ;
    }

    bool Collision::AABB(TransformComponent* one, Entity* b)
    {
        TransformComponent* two = &b->getComponent<TransformComponent>();

        bool collisionX = one->position.x + one->size.x >= two->position.x &&
             two->position.x + two->size.x >= one->position.x;
        // collision y-axis?
        bool collisionY = one->position.y + one->size.y >= two->position.y &&
            two->position.y + two->size.y >= one->position.y;
                
        bool collisionZ = one->position.z + one->size.z >= two->position.z &&
            two->position.z + two->size.z >= one->position.z;
        
        return collisionX && collisionY && collisionZ;
    }

    bool Collision::AABB(Ray* r, Entity* e)
    {
        bool collisionX = r->end.x + 0.25f >= e->getComponent<gr::TransformComponent>().position.x &&
            e->getComponent<gr::TransformComponent>().position.x + e->getComponent<gr::TransformComponent>().size.x >= r->end.x;
        // collision y-axis?
        bool collisionY = r->end.y + 0.25f >= e->getComponent<gr::TransformComponent>().position.y &&
            e->getComponent<gr::TransformComponent>().position.y + e->getComponent<gr::TransformComponent>().size.y >= r->end.y;

        bool collisionZ = r->end.z + 0.25f >= e->getComponent<gr::TransformComponent>().position.z &&
            e->getComponent<gr::TransformComponent>().position.z + e->getComponent<gr::TransformComponent>().size.z >= r->end.z;

        return collisionX && collisionY && collisionZ;
    }

}