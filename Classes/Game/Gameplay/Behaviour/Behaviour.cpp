//
//  Behaviour.cpp
//  Evolution
//
//  Created by Ricard Perez del Campo on 11/03/16.
//
//

#include "Behaviour.hpp"

namespace MelonGames
{
    namespace Evolution
    {
#pragma mark - Behaviour
        Behaviour::~Behaviour()
        {
        }
        
        void Behaviour::reset()
        {
        }
        
#pragma mark - BehaviourContainer
        BehaviourContainer::~BehaviourContainer()
        {
            for (auto behaviour : children)
            {
                delete behaviour;
            }
        }
        
        void BehaviourContainer::addBehaviour(Behaviour* behaviour)
        {
            children.push_back(behaviour);
        }
        
#pragma mark - BehaviourSequence
        BehaviourSequence::BehaviourSequence()
        : nextIndex(0)
        {
            
        }
        
        BehaviourResult BehaviourSequence::update(MapEntity* entity, float dt)
        {
            if (nextIndex < children.size())
            {
                auto nextBehaviour = *(children.begin() + nextIndex);
                auto result = nextBehaviour->update(entity, dt);
                switch (result)
                {
                    case BehaviourResult::eSuccess:
                        ++nextIndex;
                        break;
                    case BehaviourResult::eFailure:
                        return BehaviourResult::eFailure;
                        break;
                    case BehaviourResult::eRunning:
                    default:
                        break;
                }
            }
            
            if (nextIndex < children.size())
            {
                return BehaviourResult::eRunning;
            } else
            {
                return BehaviourResult::eSuccess;
            }
        }
        
        void BehaviourSequence::reset()
        {
            Base::reset();
            nextIndex = 0;
            for (auto behaviour : children)
            {
                behaviour->reset();
            }
        }
        
#pragma mark - BehaviourSelector
        BehaviourSelector::BehaviourSelector()
        : nextIndex(0)
        {
        }
        
        BehaviourResult BehaviourSelector::update(MapEntity* entity, float dt)
        {
            if (nextIndex < children.size())
            {
                auto nextBehaviour = *(children.begin() + nextIndex);
                auto result = nextBehaviour->update(entity, dt);
                switch (result)
                {
                    case BehaviourResult::eSuccess:
                        return BehaviourResult::eSuccess;
                        break;
                    case BehaviourResult::eFailure:
                        ++nextIndex;
                        break;
                    case BehaviourResult::eRunning:
                    default:
                        break;
                }
            }
            
            if (nextIndex < children.size())
            {
                return BehaviourResult::eRunning;
            } else
            {
                return BehaviourResult::eFailure;
            }
        }
        
        void BehaviourSelector::reset()
        {
            Base::reset();
            nextIndex = 0;
            for (auto behaviour : children)
            {
                behaviour->reset();
            }
        }
        
#pragma mark - BehaviourDecorator
        BehaviourDecorator::BehaviourDecorator(Behaviour* child, BehaviourResult returningResult, bool evenIfRunning)
        : child(child)
        , returningResult(returningResult)
        , evenIfRunning(evenIfRunning)
        {
        }
        
        BehaviourDecorator::~BehaviourDecorator()
        {
            delete child;
        }
        
        BehaviourResult BehaviourDecorator::update(MapEntity* entity, float dt)
        {
            auto result = child->update(entity, dt);
            
            if ((result == BehaviourResult::eRunning) && !evenIfRunning)
            {
                return BehaviourResult::eRunning;
            }
            
            return returningResult;
        }
        
        void BehaviourDecorator::reset()
        {
            Base::reset();
            child->reset();
        }
        
#pragma mark - BehaviourInverter
        BehaviourInverter::BehaviourInverter(Behaviour* child)
        : child(child)
        {
        }
        
        BehaviourInverter::~BehaviourInverter()
        {
            delete child;
        }
        
        BehaviourResult BehaviourInverter::update(MapEntity* entity, float dt)
        {
            auto result = child->update(entity, dt);
            switch (result)
            {
                case BehaviourResult::eSuccess:
                    return BehaviourResult::eFailure;
                    break;
                case BehaviourResult::eFailure:
                    return BehaviourResult::eSuccess;
                    break;
                case BehaviourResult::eRunning:
                default:
                    break;
            }
            
            return BehaviourResult::eRunning;
        }
        
        void BehaviourInverter::reset()
        {
            Base::reset();
            child->reset();
        }
        
#pragma mark - BehaviourRepeater
        BehaviourRepeater::BehaviourRepeater(Behaviour* child)
        : child(child)
        {
            
        }
        
        BehaviourRepeater::~BehaviourRepeater()
        {
            delete child;
        }
        
        BehaviourResult BehaviourRepeater::update(MapEntity* entity, float dt)
        {
            auto result = child->update(entity, dt);
            if ((result == BehaviourResult::eSuccess) || (result == BehaviourResult::eFailure))
            {
                child->reset();
            }
            
            return BehaviourResult::eRunning;
        }
        
        void BehaviourRepeater::reset()
        {
            Base::reset();
            child->reset();
        }
        
    }
}