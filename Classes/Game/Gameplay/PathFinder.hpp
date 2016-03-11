//
//  PathFinder.hpp
//  Evolution
//
//  Created by Ricard Perez del Campo on 11/03/16.
//
//

#ifndef EvolutionPathFinder_hpp
#define EvolutionPathFinder_hpp

#include <vector>
#include "math/Vec2.h"

namespace AStar
{
    class Graph;
    class PathFinder;
    class PositionConverter;
}

namespace MelonGames
{
    namespace Evolution
    {
        class Map;
        
        class PathFinder
        {
        public:
            PathFinder(Map* map);
            ~PathFinder();
            
            bool findPath(const cocos2d::Vec2& origin, const cocos2d::Vec2& destination, std::vector<cocos2d::Vec2>& waypoints) const;
            
        private:
            Map* map;
            AStar::Graph* graph;
            AStar::PathFinder* pathFinder;
            AStar::PositionConverter* positionConverter;
        };
    }
}

#endif /* EvolutionPathFinder_hpp */
