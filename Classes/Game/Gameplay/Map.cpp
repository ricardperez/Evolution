//
//  Map.cpp
//  Evolution
//
//  Created by Ricard Perez del Campo on 04/03/16.
//
//

#include "Map.hpp"
#include "MapEntity.hpp"

namespace MelonGames
{
    namespace Evolution
    {
        Map::Map()
        : updating(false)
        {
            
        }
        
        void Map::update(float dt)
        {
            updating = true;
            
            for (auto entity : entities)
            {
                entity->update(dt);
            }
            
            updating = false;
            
            for (auto& delayedEntityOperation : delayedEntityOperations)
            {
                switch (delayedEntityOperation.operation)
                {
                    case EntityDelayedOperation::Operation::eAdd:
                        addEntity(delayedEntityOperation.entity);
                        break;
                    case EntityDelayedOperation::Operation::eRemove:
                        removeEntity(delayedEntityOperation.entity);
                        break;
                    default:
                        break;
                }
            }
        }
        
        void Map::addEntity(MapEntity* entity)
        {
            if (entity->getMap() == nullptr)
            {
                entity->setMap(this);
            }
            
            if (updating)
            {
                delayedEntityOperations.push_back(EntityDelayedOperation(EntityDelayedOperation::Operation::eAdd, entity));
            } else
            {
                entities.push_back(entity);
            }
        }
        
        void Map::removeEntity(MapEntity* entity)
        {
            if (updating)
            {
                delayedEntityOperations.push_back(EntityDelayedOperation(EntityDelayedOperation::Operation::eRemove, entity));
            } else
            {
                auto it = std::find(entities.begin(), entities.end(), entity);
                if (it != entities.end())
                {
                    entities.erase(it);
                }
                
                delete entity;
            }
        }
    }
}