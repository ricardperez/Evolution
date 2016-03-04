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
        class GameScreen : public Screen
        {
        public:
            static GameScreen* create();
        };
    }
}

#endif /* GameScreen_hpp */
