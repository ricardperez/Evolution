//
//  BehaviourComponent.cpp
//  Evolution
//
//  Created by Ricard Perez del Campo on 11/03/16.
//
//

#include "BehaviourComponent.hpp"
#include "Behaviour/Behaviour.hpp"

namespace MelonGames
{
    namespace Evolution
    {
        BehaviourComponent::BehaviourComponent()
        : rootBehaviour(nullptr)
        , running(true)
        {
            
        }
        
        BehaviourComponent::~BehaviourComponent()
        {
            delete rootBehaviour;
        }
        
        void BehaviourComponent::update(float dt)
        {
            Base::update(dt);
            
            if (running)
            {
                auto result = rootBehaviour->update(entity, dt);
                running = (result == BehaviourResult::eRunning);
            }
        }
        
        void BehaviourComponent::setRootBehaviour(Behaviour* behaviour)
        {
            if (rootBehaviour && (rootBehaviour != behaviour))
            {
                delete rootBehaviour;
            }
            
            rootBehaviour = behaviour;
        }
    }
}