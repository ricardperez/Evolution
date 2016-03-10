//
//  ViewComponent.hpp
//  Evolution
//
//  Created by Ricard Perez del Campo on 04/03/16.
//
//

#ifndef ViewComponent_hpp
#define ViewComponent_hpp

#include "Component.hpp"

namespace cocos2d
{
    class Node;
}

namespace MelonGames
{
    namespace Evolution
    {
        class ViewComponent : public Component
        {
        public:
            DECLARE_TYPE_WITH_BASE_TYPE(ViewComponent, Component);
            
            void update(float dt) override;
            
        private:
            cocos2d::Node* mainNode;
        };
    }
}

#endif /* ViewComponent_hpp */
