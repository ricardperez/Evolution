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
#include "PathFinder.hpp"
#include "Screens/GameScreen.hpp"
#include "2d/CCTMXTiledMap.h"
#include "2d/CCTMXLayer.h"

namespace MelonGames
{
    namespace Evolution
    {
        Map::Map()
        : gameScreen(nullptr)
        , mapView(nullptr)
        , entityFactory(nullptr)
        , pathFinder(nullptr)
        , updating(false)
        {
            entityFactory = new MapEntityFactory();
            entityFactory->addTemplatesFromFile("bin/MapEntities.obj");
            
            mapView = new MapView(this);
            mapView->parseTiledMapObjects();
            
            pathFinder = new PathFinder(this);
            
            mapView->getTiledMap()->getLayer("ObjectsLayer")->removeFromParent();
//            mapView->getMainNode()->addChild(pathFinder->createRepresentativeNode());
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
            
            delete pathFinder;
            delete mapView;
            delete entityFactory;
        }
        
        void Map::setGameScreen(GameScreen* gameScreen)
        {
            this->gameScreen = gameScreen;
        }
        
        GameScreen* Map::getGameScreen() const
        {
            return gameScreen;
        }
        
        MapView* Map::getMapView() const
        {
            return mapView;
        }
        
        MapEntityFactory* Map::getEntityFactory() const
        {
            return entityFactory;
        }
        
        PathFinder* Map::getPathFinder() const
        {
            return pathFinder;
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