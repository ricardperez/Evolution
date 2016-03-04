//
//  GameScreen.cpp
//  Evolution
//
//  Created by Ricard Perez del Campo on 04/03/16.
//
//

#include "GameScreen.hpp"
#include "Camera/Camera.hpp"
#include "2d/CCTMXTiledMap.h"

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
        : tiledMap(nullptr)
        , camera(nullptr)
        , cameraController(nullptr)
        {
        }
        
        GameScreen::~GameScreen()
        {
            delete cameraController;
            delete camera;
        }
        
        bool GameScreen::init()
        {
            if (Base::init())
            {
                tiledMap = cocos2d::TMXTiledMap::create("tilemaps/level1.tmx");
                camera = new Camera(tiledMap);
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