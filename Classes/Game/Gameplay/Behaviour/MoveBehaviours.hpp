//
//  MoveBehaviours.hpp
//  Evolution
//
//  Created by Ricard Perez del Campo on 11/03/16.
//
//

#ifndef MoveBehaviours_hpp
#define MoveBehaviours_hpp

#include "Behaviour.hpp"

namespace MelonGames
{
    namespace Evolution
    {
#pragma mark - CheckRallyPointChangedBehaviour
        class CheckRallyPointChangedBehaviour : public Behaviour
        {
        public:
            DECLARE_TYPE_WITH_BASE_TYPE(CheckRallyPointChangedBehaviour, Behaviour);
            
            BehaviourResult update(MapEntity* entity, float dt) override;
        };
        
#pragma mark - CalculatePathBehaviour
        class CalculatePathBehaviour : public Behaviour
        {
        public:
            DECLARE_TYPE_WITH_BASE_TYPE(CalculatePathBehaviour, Behaviour);
            
            BehaviourResult update(MapEntity* entity, float dt) override;
        };
        
#pragma mark - WalkBehaviour
        class WalkBehaviour : public Behaviour
        {
        public:
            DECLARE_TYPE_WITH_BASE_TYPE(WalkBehaviour, Behaviour);
            
            BehaviourResult update(MapEntity* entity, float dt) override;
        };
    }
}

#endif /* MoveBehaviours_hpp */
