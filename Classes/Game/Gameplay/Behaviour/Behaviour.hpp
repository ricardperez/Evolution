//
//  Behaviour.hpp
//  Evolution
//
//  Created by Ricard Perez del Campo on 11/03/16.
//
//

#ifndef Behaviour_hpp
#define Behaviour_hpp

#include <vector>
#include "MelonGames/DeclaredTypes.h"

namespace MelonGames
{
    namespace Evolution
    {
        class MapEntity;
        
#pragma mark - Behaviour
        enum class BehaviourResult
        {
            eRunning,
            eSuccess,
            eFailure
        };
        
        class Behaviour
        {
        public:
            DECLARE_BASE_TYPE(Behaviour);
            
            virtual ~Behaviour();
            virtual BehaviourResult update(MapEntity* entity, float dt) = 0;
            virtual void reset();
        };
        
#pragma mark - BehaviourContainer
        class BehaviourContainer : public Behaviour
        {
        public:
            DECLARE_TYPE_WITH_BASE_TYPE(BehaviourContainer, Behaviour);
            virtual ~BehaviourContainer();
            void addBehaviour(Behaviour* behaviour);
            
        protected:
            std::vector<Behaviour*> children;
        };
        
#pragma mark - BehaviourSequence
        class BehaviourSequence : public BehaviourContainer
        {
        public:
            DECLARE_TYPE_WITH_BASE_TYPE(BehaviourSequence, BehaviourContainer);
            
            BehaviourSequence();
            BehaviourResult update(MapEntity* entity, float dt) override;
            void reset() override;
            
        private:
            int nextIndex;
        };
        
#pragma mark - BehaviourSelector
        class BehaviourSelector : public BehaviourContainer
        {
        public:
            DECLARE_TYPE_WITH_BASE_TYPE(BehaviourSelector, BehaviourContainer);
            
            BehaviourSelector();
            BehaviourResult update(MapEntity* entity, float dt) override;
            void reset() override;
            
        private:
            int nextIndex;
        };
        
#pragma mark - BehaviourDecorator
        class BehaviourDecorator : public Behaviour
        {
        public:
            DECLARE_TYPE_WITH_BASE_TYPE(BehaviourDecorator, Behaviour);
            
            BehaviourDecorator(Behaviour* child, BehaviourResult returningResult, bool evenIfRunning);
            ~BehaviourDecorator();
            BehaviourResult update(MapEntity* entity, float dt) override;
            void reset() override;
            
        private:
            Behaviour* child;
            BehaviourResult returningResult;
            bool evenIfRunning;
        };
        
#pragma mark - BehaviourInverter
        class BehaviourInverter : public Behaviour
        {
        public:
            DECLARE_TYPE_WITH_BASE_TYPE(BehaviourInverter, Behaviour);
            
            BehaviourInverter(Behaviour* child);
            ~BehaviourInverter();
            BehaviourResult update(MapEntity* entity, float dt) override;
            void reset() override;
            
        private:
            Behaviour* child;
        };
        
#pragma mark - BehaviourRepeater
        class BehaviourRepeater : public Behaviour
        {
        public:
            DECLARE_TYPE_WITH_BASE_TYPE(BehaviourRepeater, Behaviour);
            
            BehaviourRepeater(Behaviour* child);
            ~BehaviourRepeater();
            BehaviourResult update(MapEntity* entity, float dt) override;
            void reset() override;
            
        private:
            Behaviour* child;
        };
    }
    
}

#endif /* Behaviour_hpp */
