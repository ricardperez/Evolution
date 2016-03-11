//
//  MapView.hpp
//  Evolution
//
//  Created by Ricard Perez del Campo on 10/03/16.
//
//

#ifndef MapView_hpp
#define MapView_hpp

#include "math/Vec2.h"

namespace cocos2d
{
    class Node;
    class TMXTiledMap;
    class Vec3;
}

namespace MelonGames
{
    namespace Evolution
    {
        class Map;
        
        class MapView
        {
        public:
            MapView(Map* map);
            ~MapView();
            
            cocos2d::Node* getMainNode() const;
            const cocos2d::TMXTiledMap* getTiledMap() const;
            
            void parseTiledMapObjects();
            
        private:
            Map* map;
            cocos2d::Node* mainNode;
            cocos2d::TMXTiledMap* tiledMap;
        };
        
        namespace PositionProjector
        {
            cocos2d::Vec2 projectPosition(const cocos2d::Vec3& position);
        }
    }
}

#endif /* MapView_hpp */
