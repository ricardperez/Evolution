//
//  GameScreen.cpp
//  Evolution
//
//  Created by Ricard Perez del Campo on 04/03/16.
//
//

#include "GameScreen.hpp"
#include "Map.hpp"
#include "MapView.hpp"
#include "Camera/Camera.hpp"

namespace MelonGames
{
    namespace Evolution
    {
        GameScreen* GameScreen::create()
        {
            auto result = new GameScreen();
            if (result && result->init())
            {
                result->autorelease();
                return result;
            }
            
            delete result;
            return nullptr;
        }
        
        GameScreen::GameScreen()
        : map(nullptr)
        , camera(nullptr)
        , cameraController(nullptr)
        {
        }
        
        GameScreen::~GameScreen()
        {
            delete map;
            delete cameraController;
            delete camera;
        }
        
        bool GameScreen::init()
        {
            if (Base::init())
            {
                map = new Map();
                camera = new Camera(map->getMapView()->getMainNode());
                camera->setCameraSize(getContentSize());
                auto cameraNode = camera->getCameraNode();
                cameraNode->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
                cameraNode->setPosition(getContentSize()*0.5f);
                addChild(cameraNode);
                
                cameraController = new CameraController(camera);
                
                return true;
            }
            
            return false;
        }
        
        void GameScreen::onEnterTransitionDidFinish()
        {
            Base::onEnterTransitionDidFinish();
            
            schedule([&](float dt)->void {
                camera->update(dt);
            }, "updateThings");
        }
    }
}