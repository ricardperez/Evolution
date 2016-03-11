//
//  BehaviourComponent.hpp
//  Evolution
//
//  Created by Ricard Perez del Campo on 11/03/16.
//
//

#ifndef BehaviourComponent_hpp
#define BehaviourComponent_hpp

#include "Component.hpp"

namespace MelonGames
{
    namespace Evolution
    {
        class Behaviour;
        
        class BehaviourComponent : public Component
        {
        public:
            DECLARE_TYPE_WITH_BASE_TYPE(BehaviourComponent, Component);
            
            BehaviourComponent();
            ~BehaviourComponent();
            
            void update(float dt) override;
            
            void setRootBehaviour(Behaviour* behaviour);
            
        private:
            Behaviour* rootBehaviour;
            bool running;
        };
    }
}

#endif /* BehaviourComponent_hpp */
