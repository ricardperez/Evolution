//
//  TouchListenerComponent.cpp
//  Evolution
//
//  Created by Ricard Perez del Campo on 11/03/16.
//
//

#include "TouchListenerComponent.hpp"
#include "MapEntity.hpp"
#include "Map.hpp"
#include "MapView.hpp"
#include "PositionComponent.hpp"
#include "StateComponents.hpp"
#include "MelonGames/Crypto.h"
#include "base/CCDirector.h"
#include "base/CCEventDispatcher.h"
#include "base/CCEventListenerTouch.h"
#include "2d/CCDrawNode.h"
#include "math/CCGeometry.h"

namespace MelonGames
{
    namespace Evolution
    {
        namespace TouchActionsFactory
        {
            void createSetRallyPointAction(TouchListenerComponent* touchListener, const cocos2d::Touch* touch)
            {
                bool selected = touchListener->isSelected();
                touchListener->setSelected(!selected);
                if (selected)
                {
                    auto touchPosition = touch->getLocation();
                    auto walkState = touchListener->getMapEntity()->getComponent<WalkStateComponent>();
                    walkState->setRallyPoint(touchPosition);
                }
            }
        }
        
        TouchListenerComponent::TouchListenerComponent(const std::string& actionStr)
        : touchListener(nullptr)
        , drawNode(nullptr)
        , selected(false)
        {
            drawNode = cocos2d::DrawNode::create();
            drawNode->retain();
            
            static std::map<unsigned int, TouchAction> touchActions = {
                {Crypto::stringHash("setRallyPoint"), TouchActionsFactory::createSetRallyPointAction},
            };
            
            unsigned int key = Crypto::stringHash(actionStr);
            auto touchActionIt = touchActions.find(key);
            
            if (touchActionIt == touchActions.end())
            {
                CCASSERT(false, ("Could not create a TouchAction with string " + actionStr).c_str());
            }
            
            touchAction = touchActionIt->second;
            
        }
        
        TouchListenerComponent::~TouchListenerComponent()
        {
            drawNode->release();
        }
        
        void TouchListenerComponent::onAddedToMapEntity(MapEntity* entity)
        {
            Base::onAddedToMapEntity(entity);
            
            auto positionComponent = entity->getComponent<PositionComponent>();
            positionComponent->getPositionChangedSignal().Connect(this, &TouchListenerComponent::onPositionChanged);
            onPositionChanged(positionComponent);
            
            entity->getMap()->getMapView()->getMainNode()->addChild(drawNode);
            
            auto eventsListener = cocos2d::EventListenerTouchOneByOne::create();
            eventsListener->onTouchBegan = [this](const cocos2d::Touch* touch, cocos2d::Event* event) -> bool {
                return onTouchBegan(touch);
            };
            eventsListener->onTouchEnded = [this](cocos2d::Touch* touch, cocos2d::Event* event) -> void {
                onTouchEnded(touch);
            };
            eventsListener->onTouchCancelled = [this](cocos2d::Touch* touch, cocos2d::Event* event) -> void {
                onTouchEnded(touch);
            };
            cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(eventsListener, 10);
            touchListener = eventsListener;
        }
        
        void TouchListenerComponent::onRemovedFromMapEntity(MapEntity* entity)
        {
            cocos2d::Director::getInstance()->getEventDispatcher()->removeEventListener(touchListener);
            Base::onRemovedFromMapEntity(entity);
        }
        
        void TouchListenerComponent::setBounds(const cocos2d::Vec2& bottomLeft, const cocos2d::Vec2& topRight)
        {
            this->bottomLeft = bottomLeft;
            this->topRight = topRight;
            
            drawNode->clear();
            drawNode->drawRect(bottomLeft, topRight, cocos2d::Color4F::BLUE);
        }
        
        bool TouchListenerComponent::isSelected() const
        {
            return selected;
        }
        
        void TouchListenerComponent::setSelected(bool selected)
        {
            this->selected = selected;
        }
        
        void TouchListenerComponent::onPositionChanged(PositionComponent* positionComponent)
        {
            auto position = PositionProjector::projectPosition(positionComponent->getPosition());
            drawNode->setPosition(position);
        }
        
        bool TouchListenerComponent::onTouchBegan(const cocos2d::Touch* touch)
        {
            return (selected ? true : isTouchInBounds(touch));
        }
        
        void TouchListenerComponent::onTouchEnded(const cocos2d::Touch* touch)
        {
            if (selected || isTouchInBounds(touch))
            {
                touchAction(this, touch);
            }
        }
        
        bool TouchListenerComponent::isTouchInBounds(const cocos2d::Touch* touch) const
        {
            auto screenPosition = PositionProjector::projectPosition(entity->getComponent<PositionComponent>()->getPosition());
            
            cocos2d::Vec2 origin(screenPosition + bottomLeft);
            cocos2d::Size size(topRight - bottomLeft);
            cocos2d::Rect rect(origin, size);
            
            return (rect.containsPoint(touch->getLocation()));
        }
    }
}
