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
        class MapView;
        class MapEntity;
        
        class Map
        {
        public:
            Map();
            ~Map();
            
            MapView* getMapView() const;
            
            void update(float dt);
            void addEntity(MapEntity* entity);
            void removeEntity(MapEntity* entity);
            
        private:
            MapView* mapView;
            std::vector<MapEntity*> entities;
            bool updating;
            
            struct DelayedEntityOperation
            {
                enum class Operation
                {
                    eAdd,
                    eRemove
                };
                Operation operation;
                MapEntity* entity;
                
                DelayedEntityOperation(Operation operation, MapEntity* entity) : operation(operation), entity(entity) {}
            };
            std::vector<DelayedEntityOperation> delayedEntityOperations;
        };
    }
}

#endif /* Map_hpp */
