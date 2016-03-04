//
//  Camera.cpp
//  Evolution
//
//  Created by Ricard Perez del Campo on 04/03/16.
//
//

#include "Camera.hpp"
#include "base/CCDirector.h"
#include "2d/CCClippingNode.h"
#include "2d/CCDrawNode.h"
#include "base/CCEventDispatcher.h"
#include "base/CCEventListenerTouch.h"

namespace MelonGames
{
    namespace Evolution
    {
        Camera::Camera(cocos2d::Node* contentNode)
        : contentNode(contentNode)
        , clippingNode(nullptr)
        , stencilNode(nullptr)
        {
            auto parent = contentNode->getParent();
            contentNode->removeFromParent();
            
            clippingNode = cocos2d::ClippingNode::create();
            clippingNode->addChild(contentNode);
            if (parent)
            {
                parent->addChild(clippingNode);
            }
            
            auto size = cocos2d::Director::getInstance()->getWinSize()*0.5f;
            clippingNode->setContentSize(size);
            
            stencilNode = cocos2d::DrawNode::create();
            stencilNode->drawSolidRect(cocos2d::Vec2::ZERO, size, cocos2d::Color4F::MAGENTA);
            clippingNode->setStencil(stencilNode);
        }
        
        cocos2d::Node* Camera::getCameraNode() const
        {
            return clippingNode;
        }
        
        void Camera::setCameraSize(const cocos2d::Size& size)
        {
            clippingNode->setContentSize(size);
            stencilNode->clear();
            stencilNode->drawSolidRect(cocos2d::Vec2::ZERO, size, cocos2d::Color4F::MAGENTA);
        }
        
        void Camera::moveContentNode(const cocos2d::Vec2& movement)
        {
            cocos2d::Vec2 desiredPosition = (contentNode->getPosition() + movement);
            contentNode->setPosition(desiredPosition);
            
            auto bb = contentNode->getBoundingBox();
            float offsetX = 0.0f;
            float offsetY = 0.0f;
            
            if (bb.origin.x > 0.0f)
            {
                offsetX = -bb.origin.x;
            } else if ((bb.origin.x + bb.size.width) < clippingNode->getContentSize().width)
            {
                offsetX = (clippingNode->getContentSize().width - (bb.origin.x + bb.size.width));
            }
            
            if (bb.origin.y > 0.0f)
            {
                offsetY = -bb.origin.y;
            } else if ((bb.origin.y + bb.size.height) < clippingNode->getContentSize().height)
            {
                offsetY = (clippingNode->getContentSize().height - (bb.origin.y + bb.size.height));
            }
            
            desiredPosition.x += offsetX;
            desiredPosition.y += offsetY;
            
            contentNode->setPosition(desiredPosition);
        }
        
        void Camera::update(float dt)
        {
        }
        
#pragma mark - CameraController
        CameraController::CameraController(Camera* camera)
        : camera(camera)
        , eventsListener(nullptr)
        {
            auto eventsListener = cocos2d::EventListenerTouchOneByOne::create();
            
            eventsListener->onTouchBegan = [this](const cocos2d::Touch* touch, cocos2d::Event* event) -> bool {
                return onTouchBegan(touch);
            };
            
            eventsListener->onTouchMoved = [this](const cocos2d::Touch* touch, cocos2d::Event* event) -> void {
                return onTouchMoved(touch);
            };
            
            eventsListener->onTouchEnded = [this](cocos2d::Touch* touch, cocos2d::Event* event) -> void {
                onTouchEnded(touch);
            };
            
            eventsListener->onTouchCancelled = [this](cocos2d::Touch* touch, cocos2d::Event* event) -> void {
                onTouchEnded(touch);
            };
            
            this->eventsListener = eventsListener;
            
            cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(eventsListener, 10);
        }
        
        CameraController::~CameraController()
        {
            
        }
        
        bool CameraController::onTouchBegan(const cocos2d::Touch* touch)
        {
            if (touchData1.touch == nullptr)
            {
                touchData1.touch = touch;
                touchData1.startPosition = touch->getLocation();
                touchData1.lastPosition = touchData1.startPosition;
                return true;
            } else if (touchData2.touch == nullptr)
            {
                touchData2.touch = touch;
                touchData2.startPosition = touch->getLocation();
                touchData2.lastPosition = touchData2.startPosition;
                return true;
            }
            
            return false;
        }
        
        void CameraController::onTouchMoved(const cocos2d::Touch* touch)
        {
            if (touch == touchData1.touch || touch == touchData2.touch)
            {
                if ((touchData1.touch != nullptr) && (touchData2.touch != nullptr))
                {
                    applyZoom();
                }
                
                applyDrag();
                
                if (touch == touchData1.touch)
                {
                    touchData1.lastPosition = touch->getLocation();
                } else if (touch == touchData2.touch)
                {
                    touchData2.lastPosition = touch->getLocation();
                }
                
            } else
            {
                CCASSERT(false, "An unknown touch is moving?");
            }
        }
        
        void CameraController::onTouchEnded(const cocos2d::Touch* touch)
        {
            if (touch == touchData1.touch)
            {
                touchData1.touch = nullptr;
            } else if (touch == touchData2.touch)
            {
                touchData2.touch = nullptr;
            }
        }
        
        void CameraController::applyZoom()
        {
            float touchesDistance = touchData1.touch->getLocation().getDistance(touchData2.touch->getLocation());
            CCLOG("Touches distance is %.2f", touchesDistance);
        }
        
        void CameraController::applyDrag()
        {
            cocos2d::Vec2 movement;
            if ((touchData1.touch != nullptr) && (touchData2.touch == nullptr))
            {
                movement = (touchData1.touch->getLocation() - touchData1.lastPosition);
            } else if ((touchData2.touch != nullptr) && (touchData1.touch == nullptr))
            {
                movement = (touchData2.touch->getLocation() - touchData2.lastPosition);
            } else if ((touchData1.touch != nullptr) && (touchData2.touch != nullptr))
            {
                cocos2d::Vec2 movement1 = (touchData1.touch->getLocation() - touchData1.lastPosition);
                cocos2d::Vec2 movement2 = (touchData2.touch->getLocation() - touchData2.lastPosition);
                movement = ((movement1 + movement2) / 2.0f);
            }
            
            camera->moveContentNode(movement);
        }
    }
}