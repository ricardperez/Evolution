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
            
            virtual ~Component();
            virtual void onAddedToMapEntity(MapEntity* entity);
            virtual void onRemovedFromMapEntity(MapEntity* entity);
        };
    }
}

#endif /* Component_hpp */
