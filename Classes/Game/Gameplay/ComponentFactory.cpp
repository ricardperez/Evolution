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
#include "Component/TouchListenerComponent.hpp"
#include "Component/BehaviourComponent.hpp"
#include "Component/StateComponents.hpp"
#include "Behaviour/Behaviour.hpp"
#include "Behaviour/MoveBehaviours.hpp"

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
            
#pragma mark - ViewPartFactoryFunctions
            namespace ViewPartFactoryFunctions
            {
                ViewPart* createViewPartSprite(const Json::Value& json)
                {
                    std::string spriteFrameName = json["sprite"].asString();
                    auto result = new ViewPartSprite(spriteFrameName);
                    
                    return result;
                }
                
                ViewPart* createViewPartSpine(const Json::Value& json)
                {
                    auto result = new ViewPartSpine(json["skeleton"].asString(), json["atlas"].asString(), json["scale"].asFloat());
                    
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
                        {Crypto::stringHash("Spine"), createViewPartSpine},
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
            
#pragma mark - BehaviourFactoryFunctions
            namespace BehaviourFactoryFunctions
            {
                Behaviour* createBehaviour(const Json::Value& json);
                
                Behaviour* createRepeaterBehaviour(const Json::Value& json)
                {
                    auto child = createBehaviour(json["child"]);
                    auto result = new BehaviourRepeater(child);
                    return result;
                }
                
                Behaviour* createSequenceBehaviour(const Json::Value& json)
                {
                    auto result = new BehaviourSequence();
                    
                    for (auto& behaviourJson : json["children"])
                    {
                        auto child = createBehaviour(behaviourJson);
                        result->addBehaviour(child);
                    }
                    
                    return result;
                }
                
                Behaviour* createSelectorBehaviour(const Json::Value& json)
                {
                    auto result = new BehaviourSelector();
                    
                    for (auto& behaviourJson : json["children"])
                    {
                        auto child = createBehaviour(behaviourJson);
                        result->addBehaviour(child);
                    }
                    
                    return result;
                }
                
                Behaviour* createDecoratorBehaviour(const Json::Value& json)
                {
                    auto returningResultStr = json["result"].asString();
                    BehaviourResult returningResult;
                    if (returningResultStr == "success")
                    {
                        returningResult = BehaviourResult::eSuccess;
                    } else if (returningResultStr == "failure")
                    {
                        returningResult = BehaviourResult::eFailure;
                    } else if (returningResultStr == "running")
                    {
                        returningResult = BehaviourResult::eRunning;
                    } else
                    {
                        return nullptr;
                    }
                    
                    auto child = createBehaviour(json["child"]);
                    auto result = new BehaviourDecorator(child, returningResult, json["evenIfRunning"].asBool());
                    return result;
                }
                
                Behaviour* createCheckRallyPointChangedBehaviour(const Json::Value& json)
                {
                    auto result = new CheckRallyPointChangedBehaviour();
                    return result;
                }
                
                Behaviour* createCalculatePathBehaviour(const Json::Value& json)
                {
                    auto result = new CalculatePathBehaviour();
                    return result;
                }
                
                Behaviour* createWalkBehaviour(const Json::Value& json)
                {
                    auto result = new WalkBehaviour();
                    return result;
                }
                
                Behaviour* createBehaviour(const Json::Value& json)
                {
                    static std::map<unsigned int, std::function<Behaviour*(const Json::Value&)>> lambdas = {
                        {Crypto::stringHash("Repeater"), createRepeaterBehaviour},
                        {Crypto::stringHash("Sequence"), createSequenceBehaviour},
                        {Crypto::stringHash("Selector"), createSelectorBehaviour},
                        {Crypto::stringHash("Decorator"), createDecoratorBehaviour},
                        {Crypto::stringHash("CheckRallyPointChanged"), createCheckRallyPointChangedBehaviour},
                        {Crypto::stringHash("CalculatePath"), createCalculatePathBehaviour},
                        {Crypto::stringHash("Walk"), createWalkBehaviour},
                    };
                    
                    Behaviour* result = nullptr;
                    
                    std::string type = json["type"].asString();
                    unsigned int key = Crypto::stringHash(type);
                    
                    auto lambdaIt = lambdas.find(key);
                    if (lambdaIt != lambdas.end())
                    {
                        result = lambdaIt->second(json);
                        if (result)
                        {
                            return result;
                        }
                    }
                    
                    CCASSERT(false, ("Could not create a Behaviour of type " + type).c_str());
                    return nullptr;
                }
            }
            
#pragma mark - ComponentFactoryFunctions
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
            
            Component* createTouchListenerComponent(const Json::Value& json)
            {
                auto actionStr = json["action"].asString();
                auto result = new TouchListenerComponent(actionStr);
                auto& boundsJson = json["bounds"];
                result->setBounds(vec2FromJson(boundsJson[0]), vec2FromJson(boundsJson[1]));
                
                return result;
            }
            
            Component* createBehaviourComponent(const Json::Value& json)
            {
                auto result = new BehaviourComponent();
                auto behaviour = BehaviourFactoryFunctions::createBehaviour(json["behaviour"]);
                result->setRootBehaviour(behaviour);
                return result;
            }
            
            Component* createWalkStateComponent(const Json::Value& json)
            {
                auto result = new WalkStateComponent();
                result->setSpeed(json["speed"].asFloat());
                return result;
            }
        }
        
        Component* ComponentFactory::createComponent(const Json::Value &json)
        {
            static std::map<unsigned int, std::function<Component*(const Json::Value&)>> lambdas = {
                {Crypto::stringHash("Position"), ComponentFactoryFunctions::createPositionComponent},
                {Crypto::stringHash("View"), ComponentFactoryFunctions::createViewComponent},
                {Crypto::stringHash("TouchListener"), ComponentFactoryFunctions::createTouchListenerComponent},
                {Crypto::stringHash("Behaviour"), ComponentFactoryFunctions::createBehaviourComponent},
                {Crypto::stringHash("WalkState"), ComponentFactoryFunctions::createWalkStateComponent},
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