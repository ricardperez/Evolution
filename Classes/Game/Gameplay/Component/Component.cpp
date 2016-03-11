//
//  Component.cpp
//  Evolution
//
//  Created by Ricard Perez del Campo on 04/03/16.
//
//

#include "Component.hpp"
#include "MapEntity.hpp"

namespace MelonGames
{
    namespace Evolution
    {
        Component::Component()
        : entity(nullptr)
        {
            
        }
        
        Component::~Component()
        {
            
        }
        
        void Component::onAddedToMapEntity(MapEntity* entity)
        {
            this->entity = entity;
        }
        
        void Component::onRemovedFromMapEntity(MapEntity* entity)
        {
            this->entity = nullptr;
        }
        
        void Component::update(float dt)
        {
        }
        
        MapEntity* Component::getMapEntity() const
        {
            return entity;
        }
    }
}