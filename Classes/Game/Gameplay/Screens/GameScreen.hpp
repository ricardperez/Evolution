//
//  GameScreen.hpp
//  Evolution
//
//  Created by Ricard Perez del Campo on 04/03/16.
//
//

#ifndef GameScreen_hpp
#define GameScreen_hpp

#include "MelonGames/Screen.h"

namespace cocos2d
{
    class TMXTiledMap;
}

namespace MelonGames
{
    namespace Evolution
    {
        class Camera;
        class CameraController;
        
        class GameScreen : public Screen
        {
            DECLARE_TYPE_WITH_BASE_TYPE(GameScreen, Screen);
        public:
            static GameScreen* create();
            
        private:
            GameScreen();
            ~GameScreen();
            bool init() override;
            void onEnterTransitionDidFinish() override;
            
        private:
            Camera* camera;
            CameraController* cameraController;
            cocos2d::TMXTiledMap* tiledMap;
        };
    }
}

#endif /* GameScreen_hpp */
