//
//  PositionComponent.cpp
//  Evolution
//
//  Created by Ricard Perez del Campo on 04/03/16.
//
//

#include "PositionComponent.hpp"

namespace MelonGames
{
    namespace Evolution
    {
        const cocos2d::Vec3& PositionComponent::getPosition() const
        {
            return position;
        }
        
        void PositionComponent::setPosition(const cocos2d::Vec3& position)
        {
            float diffSq = position.distanceSquared(this->position);
            this->position = position;
            
            if (diffSq > std::numeric_limits<float>::epsilon())
            {
                positionChangedSignal.Emit(this);
            }
        }
        
        cocos2d::Vec2 PositionComponent::getGroundPosition() const
        {
            cocos2d::Vec2 result(position.x, position.y);
            return result;
        }
        
        void PositionComponent::setGroundPosition(const cocos2d::Vec2& position)
        {
            setPosition(cocos2d::Vec3(position.x, position.y, this->position.z));
        }
        
        Gallant::Signal1<PositionComponent*>& PositionComponent::getPositionChangedSignal()
        {
            return positionChangedSignal;
        }
    }
}