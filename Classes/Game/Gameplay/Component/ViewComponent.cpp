//
//  ViewComponent.cpp
//  Evolution
//
//  Created by Ricard Perez del Campo on 04/03/16.
//
//

#include "ViewComponent.hpp"
#include "MapEntity.hpp"
#include "PositionComponent.hpp"
#include "Map.hpp"
#include "MapView.hpp"
#include "2d/CCNode.h"
#include "2d/CCSprite.h"
#include "MelonGames/SpriteFrameHelper.h"

namespace MelonGames
{
    namespace Evolution
    {
#pragma mark - ViewComponent
        ViewComponent::ViewComponent()
        : alreadyAdded(false)
        {
        }
        
        ViewComponent::~ViewComponent()
        {
        }
        
        void ViewComponent::onAddedToMapEntity(MapEntity* entity)
        {
            Base::onAddedToMapEntity(entity);
            
            auto mapNode = entity->getMap()->getMapView()->getMainNode();
            for (auto& part : parts)
            {
                mapNode->addChild(part.viewPart->getNode());
                part.viewPart->onAddedToMapEntity(entity);
            }
            
            auto positionComponent = entity->getComponent<PositionComponent>();
            positionComponent->getPositionChangedSignal().Connect(this, &ViewComponent::onPositionChanged);
            onPositionChanged(positionComponent);
            
            alreadyAdded = true;
        }
        
        void ViewComponent::onRemovedFromMapEntity(MapEntity* entity)
        {
            auto positionComponent = entity->getComponent<PositionComponent>();
            if (positionComponent)
            {
                positionComponent->getPositionChangedSignal().Disconnect(this, &ViewComponent::onPositionChanged);
            }
            
            for (auto& part : parts)
            {
                part.viewPart->onRemovedFromMapEntity(entity);
                part.viewPart->getNode()->removeFromParent();
                if (part.owned)
                {
                    delete part.viewPart;
                }
            }
            parts.clear();
            
            Base::onRemovedFromMapEntity(entity);
        }
        
        void ViewComponent::update(float dt)
        {
            Base::update(dt);
        }
        
        void ViewComponent::addPart(ViewPart* part, bool takeOwnership)
        {
            ViewPartData partData;
            partData.viewPart = part;
            partData.owned = takeOwnership;
            parts.push_back(partData);
            
            
            if (alreadyAdded)
            {
                auto mapNode = entity->getMap()->getMapView()->getMainNode();
                mapNode->addChild(part->getNode());
                part->onAddedToMapEntity(entity);
                
                auto positionComponent = entity->getComponent<PositionComponent>();
                onPositionChanged(positionComponent);
            }
        }
        
        void ViewComponent::removePart(ViewPart* part)
        {
            auto it = std::find_if(parts.begin(), parts.end(), [part](const ViewPartData& partData)->bool {
                return (partData.viewPart == part);
            });
            CCASSERT(it != parts.end(), "Part not found!");
            
            const ViewPartData& partData = *it;
            if (entity && entity->getMap())
            {
                part->onRemovedFromMapEntity(entity);
                part->getNode()->removeFromParent();
            }
            
            if (partData.owned)
            {
                delete partData.viewPart;
            }
            parts.erase(it);
        }
        
        void ViewComponent::onPositionChanged(PositionComponent *posComponent)
        {
            auto& position = posComponent->getPosition();
            auto screenPosition = PositionProjector::projectPosition(position);
            
            for (auto& part : parts)
            {
                auto node = part.viewPart->getNode();
                node->setPosition(screenPosition + part.viewPart->getPositionOffset());
            }
        }
        
#pragma mark - ViewPart
        ViewPart::ViewPart()
        {
        }
        
        ViewPart::~ViewPart()
        {
        }
        
        void ViewPart::setPositionOffset(const cocos2d::Vec2& offset)
        {
            positionOffset = offset;
        }
        
        const cocos2d::Vec2& ViewPart::getPositionOffset() const
        {
            return positionOffset;
        }
        
        void ViewPart::onAddedToMapEntity(MapEntity* entity)
        {
        }
        
        void ViewPart::onRemovedFromMapEntity(MapEntity* entity)
        {
        }
        
#pragma mark - ViewPartSprite
        ViewPartSprite::ViewPartSprite(const std::string& spriteFrameName)
        {
            sprite = cocos2d::Sprite::createWithSpriteFrame(SpriteFrameHelper::spriteFrameOrDefault(spriteFrameName));
            sprite->retain();
        }
        
        ViewPartSprite::~ViewPartSprite()
        {
            sprite->release();
        }
        
        cocos2d::Node* ViewPartSprite::getNode()
        {
            return sprite;
        }
    }
}