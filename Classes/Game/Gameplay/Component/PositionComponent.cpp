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
        
        Gallant::Signal1<PositionComponent*>& PositionComponent::getPositionChangedSignal()
        {
            return positionChangedSignal;
        }
    }
}