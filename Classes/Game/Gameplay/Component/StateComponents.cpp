//
//  StateComponents.cpp
//  Evolution
//
//  Created by Ricard Perez del Campo on 11/03/16.
//
//

#include "StateComponents.hpp"
#include "MapEntity.hpp"
#include "PositionComponent.hpp"

namespace MelonGames
{
    namespace Evolution
    {
#pragma mark - WalkStateComponent
        WalkStateComponent::WalkStateComponent()
        : speed(10.0f)
        , nextWaypointIndex(0)
        , rallyPointChanged(false)
        {
        }
        
        void WalkStateComponent::onAddedToMapEntity(MapEntity* entity)
        {
            Base::onAddedToMapEntity(entity);
            
            auto& position = entity->getComponent<PositionComponent>()->getPosition();
            rallyPoint.set(position.x, position.y);
        }
        
        void WalkStateComponent::setRallyPoint(const cocos2d::Vec2& rallyPoint)
        {
            this->rallyPoint = rallyPoint;
            rallyPointChanged = true;
        }
        
        const cocos2d::Vec2& WalkStateComponent::getRallyPoint() const
        {
            return rallyPoint;
        }
        
        void WalkStateComponent::setSpeed(float speed)
        {
            this->speed = speed;
        }
        
        float WalkStateComponent::getSpeed() const
        {
            return speed;
        }
        
        void WalkStateComponent::setWaypoints(const std::vector<cocos2d::Vec2>& waypoints)
        {
            this->waypoints = waypoints;
        }
        
        void WalkStateComponent::resetNextWaypointIndex()
        {
            nextWaypointIndex = 0;
        }
        
        bool WalkStateComponent::hasNextWaypoint() const
        {
            return (nextWaypointIndex < waypoints.size());
        }
        
        const cocos2d::Vec2& WalkStateComponent::getNextWaypoint() const
        {
            return *(waypoints.begin() + nextWaypointIndex);
        }
        
        void WalkStateComponent::stepWaypoint()
        {
            ++nextWaypointIndex;
        }
        
        bool WalkStateComponent::isRallyPointChanged() const
        {
            return rallyPointChanged;
        }
        
        void WalkStateComponent::resetRallyPointChanged()
        {
            rallyPointChanged = false;
        }
    }
}