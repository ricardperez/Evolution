//
//  PositionComponent.hpp
//  Evolution
//
//  Created by Ricard Perez del Campo on 04/03/16.
//
//

#ifndef PositionComponent_hpp
#define PositionComponent_hpp

#include "Component.hpp"
#include "math/Vec3.h"
#include <Signal/Signal.h>

namespace MelonGames
{
    namespace Evolution
    {
        class PositionComponent : public Component
        {
        public:
            DECLARE_TYPE_WITH_BASE_TYPE(PositionComponent, Component);
            
            const cocos2d::Vec3& getPosition() const;
            void setPosition(const cocos2d::Vec3& position);
            
            Gallant::Signal1<PositionComponent*>& getPositionChangedSignal();
            
        private:
            cocos2d::Vec3 position;
            Gallant::Signal1<PositionComponent*> positionChangedSignal;
        };
    }
}

#endif /* PositionComponent_hpp */
