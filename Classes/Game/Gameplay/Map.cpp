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
#include "MapEntityFactory.hpp"

namespace MelonGames
{
    namespace Evolution
    {
        Map::Map()
        : mapView(nullptr)
        , entityFactory(nullptr)
        , updating(false)
        {
            entityFactory = new MapEntityFactory();
            entityFactory->addTemplatesFromFile("bin/MapEntities.obj");
            
            mapView = new MapView(this);
            mapView->parseTiledMapObjects();
        }
        
        Map::~Map()
        {
            for (auto entity : entities)
            {
                entity->onRemovedFromMap(this);
            }
            
            for (auto entity : entities)
            {
                delete entity;
            }
            
            for (auto& delayedEntityOperation : delayedEntityOperations)
            {
                delete delayedEntityOperation.entity;
            }
            
            delete mapView;
            
            delete entityFactory;
        }
        
        MapView* Map::getMapView() const
        {
            return mapView;
        }
        
        MapEntityFactory* Map::getEntityFactory() const
        {
            return entityFactory;
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
            delayedEntityOperations.clear();
        }
        
        void Map::addEntity(MapEntity* entity)
        {
            if (updating)
            {
                delayedEntityOperations.push_back(DelayedEntityOperation(DelayedEntityOperation::Operation::eAdd, entity));
            } else
            {
                entities.push_back(entity);
                entity->onAddedToMap(this);
            }
        }
        
        void Map::removeEntity(MapEntity* entity)
        {
            if (updating)
            {
                delayedEntityOperations.push_back(DelayedEntityOperation(DelayedEntityOperation::Operation::eRemove, entity));
            } else
            {
                entity->onRemovedFromMap(this);
                
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