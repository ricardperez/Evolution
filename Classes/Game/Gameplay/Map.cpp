//
//  Map.cpp
//  Evolution
//
//  Created by Ricard Perez del Campo on 04/03/16.
//
//

#include "Map.hpp"
#include "MapView.hpp"
#include "MapEntity.hpp"

namespace MelonGames
{
    namespace Evolution
    {
        Map::Map()
        : mapView(nullptr)
        , updating(false)
        {
            mapView = new MapView();
        }
        
        Map::~Map()
        {
            for (auto entity : entities)
            {
                delete entity;
            }
            
            for (auto& delayedEntityOperation : delayedEntityOperations)
            {
                delete delayedEntityOperation.entity;
            }
            
            delete mapView;
        }
        
        MapView* Map::getMapView() const
        {
            return mapView;
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
                    case DelayedEntityOperation::Operation::eAdd:
                        addEntity(delayedEntityOperation.entity);
                        break;
                    case DelayedEntityOperation::Operation::eRemove:
                        removeEntity(delayedEntityOperation.entity);
                        break;
                    default:
                        break;
                }
            }
        }
        
        void Map::addEntity(MapEntity* entity)
        {
            if (updating)
            {
                delayedEntityOperations.push_back(DelayedEntityOperation(DelayedEntityOperation::Operation::eAdd, entity));
            } else
            {
                entity->onAddedToMap(this);
                entities.push_back(entity);
            }
        }
        
        void Map::removeEntity(MapEntity* entity)
        {
            if (updating)
            {
                delayedEntityOperations.push_back(DelayedEntityOperation(DelayedEntityOperation::Operation::eRemove, entity));
            } else
            {
                auto it = std::find(entities.begin(), entities.end(), entity);
                if (it != entities.end())
                {
                    entities.erase(it);
                }
                
                entity->onRemovedFromMap(this);
                
                delete entity;
            }
        }
    }
}