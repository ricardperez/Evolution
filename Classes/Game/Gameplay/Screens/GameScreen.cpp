//
//  GameScreen.cpp
//  Evolution
//
//  Created by Ricard Perez del Campo on 04/03/16.
//
//

#include "GameScreen.hpp"

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
    }
}