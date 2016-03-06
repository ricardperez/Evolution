//
//  MapEntity.hpp
//  Evolution
//
//  Created by Ricard Perez del Campo on 04/03/16.
//
//

#ifndef MapEntity_hpp
#define MapEntity_hpp

namespace MelonGames
{
    namespace Evolution
    {
        class Map;
        
        class MapEntity
        {
        public:
            MapEntity();
            
            Map* getMap() const;
            void setMap(Map* map);
            
            void update(float dt);
            
            void onAddedToMap(Map* map);
            void onRemovedFromMap(Map* map);
            
        private:
            Map* map;
        };
    }
}

#endif /* MapEntity_hpp */
