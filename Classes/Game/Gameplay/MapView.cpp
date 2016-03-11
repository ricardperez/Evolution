//
//  MapView.cpp
//  Evolution
//
//  Created by Ricard Perez del Campo on 10/03/16.
//
//

#include "MapView.hpp"
#include "Map.hpp"
#include "MapEntityFactory.hpp"
#include "MapEntity.hpp"
#include "Component/PositionComponent.hpp"
#include "2d/CCNode.h"
#include "2d/CCTMXTiledMap.h"
#include "2d/CCTMXLayer.h"

namespace MelonGames
{
    namespace Evolution
    {
        MapView::MapView(Map* map)
        : map(map)
        , mainNode(nullptr)
        , tiledMap(nullptr)
        {
            tiledMap = cocos2d::TMXTiledMap::create("tilemaps/level1.tmx");
            
            mainNode = cocos2d::Node::create();
            mainNode->addChild(tiledMap);
            mainNode->setContentSize(tiledMap->getContentSize());
            mainNode->retain();
        }
        
        MapView::~MapView()
        {
            mainNode->release();
        }
        
        cocos2d::Node* MapView::getMainNode() const
        {
            return mainNode;
        }
        
        const cocos2d::TMXTiledMap* MapView::getTiledMap() const
        {
            return tiledMap;
        }
        
        void MapView::parseTiledMapObjects()
        {
            auto& objectGroups = tiledMap->getObjectGroups();
            for (cocos2d::TMXObjectGroup* objectGroup : objectGroups)
            {
                for (auto& object : objectGroup->getObjects())
                {
                    auto& objectMap = object.asValueMap();
                    auto objectName = objectMap["name"].asString();
                    auto mapEntity = map->getEntityFactory()->createEntity(objectName);
                    mapEntity->getComponent<PositionComponent>()->setPosition(cocos2d::Vec3(objectMap["x"].asFloat(), objectMap["y"].asFloat(), objectMap["z"].asFloat()));
                    map->addEntity(mapEntity);
                    
                }
            }
        }
        
        namespace PositionProjector
        {
            cocos2d::Vec2 projectPosition(const cocos2d::Vec3& position)
            {
                cocos2d::Vec2 result(position.x, position.y + position.z);
                return result;
            }
        }
    }
}