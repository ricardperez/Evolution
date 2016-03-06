//
//  MapEntity.cpp
//  Evolution
//
//  Created by Ricard Perez del Campo on 04/03/16.
//
//

#include "MapEntity.hpp"

namespace MelonGames
{
    namespace Evolution
    {
        MapEntity::MapEntity()
        : map(nullptr)
        {
            
        }
        
        Map* MapEntity::getMap() const
        {
            return map;
        }
        
        void MapEntity::setMap(Map* map)
        {
            this->map = map;
        }
        
        void MapEntity::update(float dt)
        {
            
        }
        
        void MapEntity::onAddedToMap(Map* map)
        {
            
        }
        
        void MapEntity::onRemovedFromMap(Map* map)
        {
            
        }
    }
}