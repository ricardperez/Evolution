//
//  Component.hpp
//  Evolution
//
//  Created by Ricard Perez del Campo on 04/03/16.
//
//

#ifndef Component_hpp
#define Component_hpp

#include <MelonGames/DeclaredTypes.h>

namespace MelonGames
{
    namespace Evolution
    {
        class MapEntity;
        
        class Component
        {
        public:
            DECLARE_BASE_TYPE(Component);
            
            Component();
            virtual ~Component();
            
            virtual void onAddedToMapEntity(MapEntity* entity);
            virtual void onRemovedFromMapEntity(MapEntity* entity);
            virtual void update(float dt);
            
            MapEntity* getMapEntity() const;
            
        protected:
            MapEntity* entity;
        };
    }
}

#endif /* Component_hpp */
