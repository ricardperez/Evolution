//
//  Camera.hpp
//  Evolution
//
//  Created by Ricard Perez del Campo on 04/03/16.
//
//

#ifndef Camera_hpp
#define Camera_hpp

#include "math/CCGeometry.h"

namespace cocos2d
{
    class Node;
    class DrawNode;
    class ClippingNode;
    class Touch;
    class EventListener;
}

namespace MelonGames
{
    namespace Evolution
    {
        class Camera
        {
        public:
            Camera(cocos2d::Node* contentNode);
            
            cocos2d::Node* getCameraNode() const;
            void setCameraSize(const cocos2d::Size& size);
            void moveContentNode(const cocos2d::Vec2& movement);
            
            void update(float dt);
            
        private:
            cocos2d::Node* contentNode;
            cocos2d::ClippingNode* clippingNode;
            cocos2d::DrawNode* stencilNode;
        };
        
        class CameraController
        {
        public:
            CameraController(Camera* camera);
            ~CameraController();
            
        private:
            bool onTouchBegan(const cocos2d::Touch* touch);
            void onTouchMoved(const cocos2d::Touch* touch);
            void onTouchEnded(const cocos2d::Touch* touch);
            
            void applyZoom();
            void applyDrag();
            
        private:
            Camera* camera;
            cocos2d::EventListener* eventsListener;
            
            struct TouchData
            {
                const cocos2d::Touch* touch;
                cocos2d::Vec2 lastPosition;
                cocos2d::Vec2 startPosition;
                
                TouchData() : touch(nullptr) {}
            };
            TouchData touchData1;
            TouchData touchData2;
        };
    }
}

#endif /* Camera_hpp */
