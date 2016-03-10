//
//  ViewComponent.cpp
//  Evolution
//
//  Created by Ricard Perez del Campo on 04/03/16.
//
//

#include "ViewComponent.hpp"
#include "MapEntity.hpp"
#include "PositionComponent.hpp"
#include "MapView.hpp"
#include "2d/CCNode.h"

namespace MelonGames
{
    namespace Evolution
    {
        void ViewComponent::update(float dt)
        {
            Base::update(dt);
            
            auto positionComponent = entity->getComponent<PositionComponent>();
            auto screenPosition = PositionProjector::projectPosition(positionComponent->getPosition());
            mainNode->setPosition(screenPosition);
        }
    }
}