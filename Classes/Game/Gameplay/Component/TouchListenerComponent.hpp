//
//  TouchListenerComponent.hpp
//  Evolution
//
//  Created by Ricard Perez del Campo on 11/03/16.
//
//

#ifndef TouchListenerComponent_hpp
#define TouchListenerComponent_hpp

#include "Component.hpp"
#include "math/Vec2.h"
#include <functional>

namespace cocos2d
{
    class EventListener;
    class DrawNode;
    class Touch;
}

namespace MelonGames
{
    namespace Evolution
    {
        class PositionComponent;
        
        class TouchListenerComponent : public Component
        {
        public:
            DECLARE_TYPE_WITH_BASE_TYPE(TouchListenerComponent, Component);
            
            TouchListenerComponent(const std::string& actionStr);
            ~TouchListenerComponent();
            
            void onAddedToMapEntity(MapEntity* entity) override;
            void onRemovedFromMapEntity(MapEntity* entity) override;
            
            void setBounds(const cocos2d::Vec2& bottomLeft, const cocos2d::Vec2& topRight);
            
            bool isSelected() const;
            void setSelected(bool selected);
            
            bool isTouchInBounds(const cocos2d::Touch* touch) const;
            
        private:
            void onPositionChanged(PositionComponent* positionComponent);
            
            bool onTouchBegan(const cocos2d::Touch* touch);
            void onTouchEnded(const cocos2d::Touch* touch);
            
        private:
            cocos2d::Vec2 bottomLeft;
            cocos2d::Vec2 topRight;
            
            cocos2d::EventListener* touchListener;
            cocos2d::DrawNode* drawNode;
            
            typedef std::function<void(TouchListenerComponent*, const cocos2d::Touch*)> TouchAction;
            TouchAction touchAction;
            
            bool selected;
        };
    }
}

#endif /* TouchListenerComponent_hpp */
