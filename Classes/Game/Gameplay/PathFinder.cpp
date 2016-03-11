//
//  PathFinder.cpp
//  Evolution
//
//  Created by Ricard Perez del Campo on 11/03/16.
//
//

#include "PathFinder.hpp"
#include "Map.hpp"
#include "MapView.hpp"
#include "PathFinder/Graph.hpp"
#include "PathFinder/PathFinder.hpp"
#include "PathFinder/PositionConverter.hpp"
#include "PathFinder/Position.hpp"
#include "2d/CCTMXTiledMap.h"
#include "2d/CCTMXLayer.h"
#include "2d/CCDrawNode.h"
#include "2d/CCLayer.h"

namespace MelonGames
{
    namespace Evolution
    {
        PathFinder::PathFinder(Map* map)
        : map(map)
        , graph(nullptr)
        , pathFinder(nullptr)
        , positionConverter(nullptr)
        {
            auto mapNode = map->getMapView()->getMainNode();
            auto tiledMap = map->getMapView()->getTiledMap();
            graph = new AStar::Graph(tiledMap->getMapSize().width, tiledMap->getMapSize().height);
            pathFinder = new AStar::PathFinder(*graph);
            positionConverter = new AStar::PositionConverter();
            positionConverter->setGraphSize(tiledMap->getMapSize().width, tiledMap->getMapSize().height);
            positionConverter->setScreenSize(mapNode->getContentSize().width, mapNode->getContentSize().height);
            
            auto objectsLayer = tiledMap->getLayer("ObjectsLayer");
            for (int x=0; x<graph->getWidth(); ++x)
            {
                for (int y=0; y<graph->getHeight(); ++y)
                {
                    auto tile = objectsLayer->getTileAt(cocos2d::Vec2(x,y));
                    if (tile != nullptr)
                    {
                        graph->markPositionAsNonWalkable(x, (graph->getHeight()-1 - y));
                    }
                }
            }
        }
        
        PathFinder::~PathFinder()
        {
            delete positionConverter;
            delete pathFinder;
            delete graph;
        }
        
        bool PathFinder::findPath(const cocos2d::Vec2& origin, const cocos2d::Vec2& destination, std::vector<cocos2d::Vec2>& waypoints) const
        {
            auto graphOrigin = positionConverter->graphPositionFromScreenPosition(AStar::ScreenPosition(origin.x, origin.y));
            auto graphDest = positionConverter->graphPositionFromScreenPosition(AStar::ScreenPosition(destination.x, destination.y));
            
            std::vector<AStar::GraphPosition> graphPositions;
            if (pathFinder->findPathWaypoints(graphOrigin, graphDest, graphPositions))
            {
                for (auto& graphPosition : graphPositions)
                {
                    auto screenPosition = positionConverter->screenPositionFromGraphPosition(graphPosition);
                    waypoints.push_back(cocos2d::Vec2(screenPosition.x, screenPosition.y));
                }
                return true;
            }
            
            return false;
        }
        
        cocos2d::Node* PathFinder::createRepresentativeNode() const
        {
            auto& screenSize = map->getMapView()->getMainNode()->getContentSize();
            float cellsWidth = (screenSize.width / graph->getWidth());
            float cellsHeight = (screenSize.height / graph->getHeight());
            
            auto result = cocos2d::Node::create();
            
            auto traversableColor = cocos2d::Color4B::WHITE;
            traversableColor.a = 0;
            auto intraversableColor = cocos2d::Color4B::WHITE;
            intraversableColor.a = 75;
            
            auto drawNode = cocos2d::DrawNode::create();
            result->addChild(drawNode, 10);
            
            for (int y=0; y<graph->getHeight(); ++y)
            {
                for (int x=0; x<graph->getWidth(); ++x)
                {
                    bool traversable = graph->isPositionWalkable(x, y);
                    
                    auto layer = cocos2d::LayerColor::create((traversable ? traversableColor : intraversableColor), cellsWidth, cellsHeight);
                    auto screenPosition = positionConverter->screenPositionFromGraphPosition(AStar::GraphPosition(x,y));
                    layer->setPosition(screenPosition.x, screenPosition.y);
                    result->addChild(layer);
                }
            }
            
            cocos2d::Color4F linesColor(0.5f, 0.5f, 0.5f, 0.3f);
            for (int x=0; x<graph->getWidth(); ++x)
            {
                drawNode->drawLine(cocos2d::Vec2(x*cellsWidth, 0.0f), cocos2d::Vec2(x*cellsWidth, screenSize.height), linesColor);
            }
            for (int y=0; y<graph->getHeight(); ++y)
            {
                drawNode->drawLine(cocos2d::Vec2(0.0f, y*cellsHeight), cocos2d::Vec2(screenSize.width, y*cellsHeight), linesColor);
            }
            
            return result;
        }
    }
}