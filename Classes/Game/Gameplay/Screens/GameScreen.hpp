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

namespace MelonGames
{
    namespace Evolution
    {
        class Map;
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
            Map* map;
            Camera* camera;
            CameraController* cameraController;
        };
    }
}

#endif /* GameScreen_hpp */
