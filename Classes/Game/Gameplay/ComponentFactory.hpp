//
//  ComponentFactory.hpp
//  Evolution
//
//  Created by Ricard Perez del Campo on 11/03/16.
//
//

#ifndef ComponentFactory_hpp
#define ComponentFactory_hpp

namespace Json
{
    class Value;
}

namespace MelonGames
{
    namespace Evolution
    {
        class Component;
        
        class ComponentFactory
        {
        public:
            Component* createComponent(const Json::Value& json);
        };
    }
}

#endif /* ComponentFactory_hpp */
