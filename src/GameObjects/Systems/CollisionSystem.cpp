#include "CollisionSystem.h"

#include <string>

#include "../GameObject.h"
#include "../../Logger/Logger.h"
class CollisionStayEvent;
class CollisionEnterEvent;
class BoxCollider2DComponent;
class TransformComponent;

bool CollisionSystem::CheckAABBCollision(double aX, double aY, double aW, double aH, double bX, double bY, double bW,
                                         double bH)
{
    return (
        aX < bX + bW &&
        aX + aW > bX &&
        aY < bY + bH &&
        aY + aH > bY
    );
}

glm::vec2 CollisionSystem::GetCollisionDirection(double aX, double aY, double aW, double aH,
                                          double bX, double bY, double bW, double bH)
{
    // Check if there's a collision
    bool collision = (
        aX < bX + bW &&
        aX + aW > bX &&
        aY < bY + bH &&
        aY + aH > bY
    );

    if (!collision)
    {
        return glm::vec2(0.0, 0.0); // No collision, return zero vector
    }

    // Calculate the overlap in both X and Y directions
    double overlapX = std::min(aX + aW, bX + bW) - std::max(aX, bX);
    double overlapY = std::min(aY + aH, bY + bH) - std::max(aY, bY);

    // Determine the collision direction based on the overlaps
    glm::vec2 direction(0.0, 0.0);

    if (overlapX < overlapY)
    {
        // Collision is more horizontal, set X direction
        direction.x = (aX < bX) ? -1.0f : 1.0f; // a is to the left or right of b
    }
    else
    {
        // Collision is more vertical, set Y direction
        direction.y = (aY < bY) ? -1.0f : 1.0f; // a is above or below b
    }

    return direction; // Return the collision direction vector
}

void CollisionSystem::CalculateCollisions(std::vector<std::unique_ptr<GameObject>>& m_gameObjects)
{
    for (auto i = m_gameObjects.begin(); i != m_gameObjects.end(); ++i)
    {
        const std::unique_ptr<GameObject>& a = *i;

        if (!a->HasComponent<TransformComponent>() || !a->HasComponent<BoxCollider2DComponent>())
            continue;

        TransformComponent* aTransform = a->GetComponent<TransformComponent>();
        BoxCollider2DComponent* aCollider = a->GetComponent<BoxCollider2DComponent>();

        // Loop all the entities that still need to be checked (to the right of i)
        for (auto j = i; j != m_gameObjects.end(); ++j)
        {
            const std::unique_ptr<GameObject>& b = *j;

            if (!b->HasComponent<TransformComponent>() || !b->HasComponent<BoxCollider2DComponent>())
                continue;

            // Bypass if we are trying to test the same entity
            if (a == b)
                continue;

            auto bTransform = b->GetComponent<TransformComponent>();
            auto bCollider = b->GetComponent<BoxCollider2DComponent>();

            // Perform the AABB collision check between entities a and b
            bool collisionHappened = CheckAABBCollision(
                aTransform->Position.x + aCollider->Offset.x,
                aTransform->Position.y + aCollider->Offset.y,
                aCollider->Width,
                aCollider->Height,
                bTransform->Position.x + bCollider->Offset.x,
                bTransform->Position.y + bCollider->Offset.y,
                bCollider->Width,
                bCollider->Height
            );

            bool wasColliding = false;
            auto range = m_objectsColliding.equal_range(a->GetId());
            for (auto it = range.first; it != range.second; ++it)
            {
                if (it->second == b->GetId())
                {
                    wasColliding = true;
                    break;
                }
            }
            if (collisionHappened)
            {
                if (!wasColliding)
                {
                    Logger::Log("Entities " + a->Name + " and " + b->Name + " started colliding!(Ids:"
                        + std::to_string(a->GetId()) + ", " + std::to_string(b->GetId()) + ")");
                    glm::vec2 collisionDirectionA = GetCollisionDirection(
                        aTransform->Position.x + aCollider->Offset.x,
                        aTransform->Position.y + aCollider->Offset.y,
                        aCollider->Width,
                        aCollider->Height,
                        bTransform->Position.x + bCollider->Offset.x,
                        bTransform->Position.y + bCollider->Offset.y,
                        bCollider->Width,
                        bCollider->Height
                    );
                    a->LocalEventBus.EmitEvent<CollisionEnterEvent>(b, -collisionDirectionA);
                    b->LocalEventBus.EmitEvent<CollisionEnterEvent>(a, collisionDirectionA);
                    a->LocalEventBus.EmitEvent<CollisionStayEvent>(b);
                    b->LocalEventBus.EmitEvent<CollisionStayEvent>(a);
                    m_objectsColliding.insert(std::make_pair(a->GetId(), b->GetId()));
                }
                else
                {
                    a->LocalEventBus.EmitEvent<CollisionStayEvent>(b);
                    b->LocalEventBus.EmitEvent<CollisionStayEvent>(a);
                }
            }
            else
            {
                if (wasColliding)
                {
                    Logger::Log(
                        "Entities " + std::to_string(a->GetId()) + " and " + std::to_string(b->GetId()) +
                        " exit colliding!");
                    a->LocalEventBus.EmitEvent<CollisionExitEvent>(b);
                    b->LocalEventBus.EmitEvent<CollisionExitEvent>(a);

                    // Find and remove the specific key-value pair
                    auto range = m_objectsColliding.equal_range(a->GetId());
                    for (auto it = range.first; it != range.second; ++it)
                    {
                        if (it->second == b->GetId())
                        {
                            m_objectsColliding.erase(it);
                            break;
                        }
                    }
                }
            }
        }
    }
}
