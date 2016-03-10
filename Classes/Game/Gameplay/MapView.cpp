//
//  MapView.cpp
//  Evolution
//
//  Created by Ricard Perez del Campo on 10/03/16.
//
//

#include "MapView.hpp"
#include "2d/CCNode.h"
#include "2d/CCTMXTiledMap.h"

namespace MelonGames
{
    namespace Evolution
    {
        MapView::MapView()
        : mainNode(nullptr)
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