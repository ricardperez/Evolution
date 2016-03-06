//
//  Map.hpp
//  Evolution
//
//  Created by Ricard Perez del Campo on 04/03/16.
//
//

#ifndef Map_hpp
#define Map_hpp

#include <vector>

namespace MelonGames
{
    namespace Evolution
    {
        class MapEntity;
        
        class Map
        {
        public:
            Map();
            
            void update(float dt);
            void addEntity(MapEntity* entity);
            void removeEntity(MapEntity* entity);
            
        private:
            std::vector<MapEntity*> entities;
            bool updating;
            
            struct EntityDelayedOperation
            {
                enum class Operation
                {
                    eAdd,
                    eRemove
                };
                Operation operation;
                MapEntity* entity;
                
                EntityDelayedOperation(Operation operation, MapEntity* entity) : operation(operation), entity(entity) {}
            };
            std::vector<EntityDelayedOperation> delayedEntityOperations;
        };
    }
}

#endif /* Map_hpp */
