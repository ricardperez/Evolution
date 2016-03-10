//
//  MapEntity.hpp
//  Evolution
//
//  Created by Ricard Perez del Campo on 04/03/16.
//
//

#ifndef MapEntity_hpp
#define MapEntity_hpp

#include "Component/Component.hpp"
#include <vector>

namespace MelonGames
{
    namespace Evolution
    {
        class Map;
        class Component;
        
        class MapEntity
        {
        public:
            MapEntity();
            ~MapEntity();
            
            Map* getMap() const;
            
            void update(float dt);
            
            void onAddedToMap(Map* map);
            void onRemovedFromMap(Map* map);
            
            void addComponent(Component* component);
            void removeComponent(Component* component);
            
            template <class T>
            const T* getComponent() const;
            
            template <class T>
            T* getComponent();
            
        private:
            Map* map;
            bool updating;
            std::vector<Component*> components;
            
            struct DelayedComponentOperation
            {
                enum class Operation
                {
                    eAdd,
                    eRemove
                };
                Operation operation;
                Component* component;
                
                DelayedComponentOperation(Operation operation, Component* component) : operation(operation), component(component) {}
            };
            std::vector<DelayedComponentOperation> delayedComponentOperations;
        };
    }
}

namespace MelonGames
{
    namespace Evolution
    {
        template <class T>
        const T* MapEntity::getComponent() const
        {
            for (auto component : components)
            {
                if (component->isA<T>())
                {
                    return static_cast<const T*>(component);
                }
            }
            
            return nullptr;
        }
        
        template <class T>
        T* MapEntity::getComponent()
        {
            for (auto component : components)
            {
                if (component->isA<T>())
                {
                    return static_cast<T*>(component);
                }
            }
            
            return nullptr;
        }
    }
}

#endif /* MapEntity_hpp */
