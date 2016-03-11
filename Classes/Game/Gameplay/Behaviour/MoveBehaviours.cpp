//
//  MoveBehaviours.cpp
//  Evolution
//
//  Created by Ricard Perez del Campo on 11/03/16.
//
//

#include "MoveBehaviours.hpp"
#include "MapEntity.hpp"
#include "Map.hpp"
#include "PathFinder.hpp"
#include "Component/StateComponents.hpp"
#include "Component/PositionComponent.hpp"

namespace MelonGames
{
    namespace Evolution
    {
#pragma mark - CheckRallyPointChangedBehaviour
        BehaviourResult CheckRallyPointChangedBehaviour::update(MapEntity* entity, float dt)
        {
            auto walkState = entity->getComponent<WalkStateComponent>();
            if (walkState->isRallyPointChanged())
            {
                walkState->resetRallyPointChanged();
                return BehaviourResult::eSuccess;
            }
            
            return BehaviourResult::eFailure;
        }
        
#pragma mark - CalculatePathBehaviour
        BehaviourResult CalculatePathBehaviour::update(MapEntity* entity, float dt)
        {
            auto walkState = entity->getComponent<WalkStateComponent>();
            auto posComponent = entity->getComponent<PositionComponent>();
            
            auto pathFinder = entity->getMap()->getPathFinder();
            std::vector<cocos2d::Vec2> waypoints;
            bool pathExists = pathFinder->findPath(posComponent->getGroundPosition(), walkState->getRallyPoint(), waypoints);
            
            if (pathExists)
            {
                walkState->setWaypoints(waypoints);
                walkState->resetNextWaypointIndex();
                return BehaviourResult::eSuccess;
            } else
            {
                return BehaviourResult::eFailure;
            }
        }
        
#pragma mark - WalkBehaviour
        BehaviourResult WalkBehaviour::update(MapEntity* entity, float dt)
        {
            auto walkState = entity->getComponent<WalkStateComponent>();
            auto positionComponent = entity->getComponent<PositionComponent>();
            float moveDistance = (walkState->getSpeed() * dt);
            while (moveDistance > std::numeric_limits<float>::epsilon())
            {
                if (walkState->hasNextWaypoint())
                {
                    auto& nextWaypoint = walkState->getNextWaypoint();
                    cocos2d::Vec2 position = positionComponent->getGroundPosition();
                    
                    cocos2d::Vec2 diffVector = (nextWaypoint - position);
                    if (moveDistance*moveDistance < diffVector.getLengthSq())
                    {
                        cocos2d::Vec2 moveVector = (diffVector.getNormalized() * moveDistance);
                        positionComponent->setGroundPosition(position + moveVector);
                        moveDistance = 0.0f;
                    } else
                    {
                        positionComponent->setGroundPosition(nextWaypoint);
                        moveDistance -= diffVector.getLength();
                        walkState->stepWaypoint();
                    }
                } else
                {
                    return BehaviourResult::eSuccess;
                }
            }
            
            return BehaviourResult::eRunning;
        }
    }
}