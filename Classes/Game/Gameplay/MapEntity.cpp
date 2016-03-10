//
//  MapEntity.cpp
//  Evolution
//
//  Created by Ricard Perez del Campo on 04/03/16.
//
//

#include "MapEntity.hpp"
#include <cassert>

namespace MelonGames
{
    namespace Evolution
    {
        MapEntity::MapEntity()
        : map(nullptr)
        {
            
        }
        
        MapEntity::~MapEntity()
        {
            for (auto component : components)
            {
                delete component;
            }
            
            for (auto& delayedComponentOperation : delayedComponentOperations)
            {
                delete delayedComponentOperation.component;
            }
        }
        
        Map* MapEntity::getMap() const
        {
            return map;
        }
        
        void MapEntity::update(float dt)
        {
            updating = true;
            
            for (auto component : components)
            {
                component->update(dt);
            }
            
            updating = false;
            
            for (auto& delayedComponentOperation : delayedComponentOperations)
            {
                switch (delayedComponentOperation.operation)
                {
                    case DelayedComponentOperation::Operation::eAdd:
                        addComponent(delayedComponentOperation.component);
                        break;
                    case DelayedComponentOperation::Operation::eRemove:
                    default:
                        removeComponent(delayedComponentOperation.component);
                        break;
                }
            }
            delayedComponentOperations.clear();
        }
        
        void MapEntity::onAddedToMap(Map* map)
        {
            this->map = map;
            for (auto component : components)
            {
                component->onAddedToMapEntity(this);
            }
        }
        
        void MapEntity::onRemovedFromMap(Map* map)
        {
            for (auto component : components)
            {
                component->onRemovedFromMapEntity(this);
                delete component;
            }
            components.clear();
        }
        
        void MapEntity::addComponent(Component* component)
        {
            if (updating)
            {
                delayedComponentOperations.push_back(DelayedComponentOperation(DelayedComponentOperation::Operation::eAdd, component));
            } else
            {
                components.push_back(component);
                if (map)
                {
                    component->onAddedToMapEntity(this);
                }
            }
        }
        
        void MapEntity::removeComponent(Component* component)
        {
            if (updating)
            {
                delayedComponentOperations.push_back(DelayedComponentOperation(DelayedComponentOperation::Operation::eRemove, component));
            } else
            {
                auto it = std::find(components.begin(), components.end(), component);
                assert(it != components.end());
                components.erase(it);
                if (map)
                {
                    component->onRemovedFromMapEntity(this);
                }
                
                delete component;
            }
        }
    }
}