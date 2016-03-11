//
//  StateComponents.hpp
//  Evolution
//
//  Created by Ricard Perez del Campo on 11/03/16.
//
//

#ifndef StateComponents_hpp
#define StateComponents_hpp

#include "Component.hpp"
#include "math/Vec2.h"
#include <vector>

namespace MelonGames
{
    namespace Evolution
    {
#pragma mark - WalkStateComponent
        class WalkStateComponent : public Component
        {
        public:
            DECLARE_TYPE_WITH_BASE_TYPE(WalkStateComponent, Component);
            
            WalkStateComponent();
            
            void onAddedToMapEntity(MapEntity* entity) override;
            
            void setRallyPoint(const cocos2d::Vec2& rallyPoint);
            const cocos2d::Vec2& getRallyPoint() const;
            
            void setSpeed(float speed);
            float getSpeed() const;
            
            void setWaypoints(const std::vector<cocos2d::Vec2>& waypoints);
            void resetNextWaypointIndex();
            bool hasNextWaypoint() const;
            const cocos2d::Vec2& getNextWaypoint() const;
            void stepWaypoint();
            
            bool isRallyPointChanged() const;
            void resetRallyPointChanged();
            
        private:
            cocos2d::Vec2 rallyPoint;
            float speed;
            std::vector<cocos2d::Vec2> waypoints;
            int nextWaypointIndex;
            bool rallyPointChanged;
        };
    }
}

#endif /* StateComponents_hpp */
