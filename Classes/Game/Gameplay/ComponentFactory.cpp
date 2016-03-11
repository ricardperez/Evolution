//
//  ComponentFactory.cpp
//  Evolution
//
//  Created by Ricard Perez del Campo on 11/03/16.
//
//

#include "ComponentFactory.hpp"
#include "MelonGames/Crypto.h"
#include "Component/PositionComponent.hpp"
#include "Component/ViewComponent.hpp"

#include "2d/CCNode.h"
#include "math/Vec2.h"
#include <jsoncpp/value.h>

namespace MelonGames
{
    namespace Evolution
    {
        namespace ComponentFactoryFunctions
        {
            cocos2d::Vec2 vec2FromJson(const Json::Value& json)
            {
                cocos2d::Vec2 result(json[0].asFloat(), json[1].asFloat());
                return result;
            }
            
            namespace ViewPartFactoryFunctions
            {
                ViewPart* createViewPartSprite(const Json::Value& json)
                {
                    std::string spriteFrameName = json["sprite"].asString();
                    ViewPartSprite* result = new ViewPartSprite(spriteFrameName);
                    
                    return result;
                }
                
                void configureViewPartBase(ViewPart* viewPart, const Json::Value& json)
                {
                    viewPart->setPositionOffset(vec2FromJson(json["offset"]));
                    viewPart->getNode()->setAnchorPoint(vec2FromJson(json["anchorPoint"]));
                }
                
                ViewPart* createViewPart(const Json::Value& json)
                {
                    static std::map<unsigned int, std::function<ViewPart*(const Json::Value&)>> lambdas = {
                        {Crypto::stringHash("Sprite"), createViewPartSprite},
                    };
                    
                    ViewPart* result = nullptr;
                    
                    std::string type = json["type"].asString();
                    unsigned int key = Crypto::stringHash(type);
                    
                    auto lambdaIt = lambdas.find(key);
                    if (lambdaIt != lambdas.end())
                    {
                        result = lambdaIt->second(json);
                    }
                    
                    if (result)
                    {
                        configureViewPartBase(result, json);
                        return result;
                    }
                    
                    CCASSERT(false, ("Could not create a ViewPart of type " + type).c_str());
                    return nullptr;
                }
            }
            
            Component* createPositionComponent(const Json::Value& json)
            {
                return new PositionComponent();
            }
            
            Component* createViewComponent(const Json::Value& json)
            {
                auto result = new ViewComponent();
                
                for (const auto& partJson : json["parts"])
                {
                    ViewPart* part = ViewPartFactoryFunctions::createViewPart(partJson);
                    if (part != nullptr)
                    {
                        result->addPart(part, true);
                    }
                    else
                    {
                        delete result;
                        return nullptr;
                    }
                }
                
                return result;
            }
        }
        
        Component* ComponentFactory::createComponent(const Json::Value &json)
        {
            static std::map<unsigned int, std::function<Component*(const Json::Value&)>> lambdas = {
                {Crypto::stringHash("Position"), ComponentFactoryFunctions::createPositionComponent},
                {Crypto::stringHash("View"), ComponentFactoryFunctions::createViewComponent},
            };
            
            std::string type = json["type"].asString();
            unsigned int key = Crypto::stringHash(type);
            
            auto lambdaIt = lambdas.find(key);
            if (lambdaIt != lambdas.end())
            {
                return lambdaIt->second(json);
            }
            
            CCASSERT(false, ("Could not create a component of type " + type).c_str());
            
            return nullptr;
        }
    }
}