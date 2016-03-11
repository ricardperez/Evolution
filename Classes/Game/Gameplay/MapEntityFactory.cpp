//
//  MapEntityFactory.cpp
//  Evolution
//
//  Created by Ricard Perez del Campo on 11/03/16.
//
//

#include "MapEntityFactory.hpp"
#include "ComponentFactory.hpp"
#include "MapEntity.hpp"
#include "MelonGames/JsonUtil.h"
#include "MelonGames/Crypto.h"
#include "platform/CCFileUtils.h"

namespace MelonGames
{
    namespace Evolution
    {
#pragma mark - MapEntityTemplate
        Json::Value* MapEntityTemplate::getComponentJson(const std::string& componentType)
        {
            for (Json::Value& componentJson : json["components"])
            {
                if (componentType == componentJson["type"].asString())
                {
                    return &componentJson;
                }
            }
            
            return nullptr;
        }
        
#pragma mark - MapEntityFactory
        MapEntityFactory::MapEntityFactory()
        : componentFactory(nullptr)
        {
            componentFactory = new ComponentFactory();
        }
        
        MapEntityFactory::~MapEntityFactory()
        {
            delete componentFactory;
        }
        
        void MapEntityFactory::mergeTemplate(const MapEntityTemplate& superTemplate, MapEntityTemplate& baseTemplate) const
        {
            for (const auto& superComponentJson : superTemplate.json["components"])
            {
                std::string componentName = superComponentJson["type"].asString();
                
                if (auto baseComponentJsonPtr = baseTemplate.getComponentJson(componentName))
                {
                    JsonUtil::mergeContentToBase(superComponentJson, *baseComponentJsonPtr, JsonUtil::MergeType::eAutomatic);
                }
                else
                {
                    baseTemplate.json["components"].append(superComponentJson);
                }
            }
        }
        
        void MapEntityFactory::addTemplatesFromFile(const std::string& filename)
        {
            auto& templates = templatesByFile[filename];
            
            Json::Value json;
            std::string contents = cocos2d::FileUtils::getInstance()->getStringFromFile(filename);
            Json::Reader reader;
            reader.parse(contents, json);
            
            const Json::Value& objectsJson = json["obj"];
            
            for (const auto objectJson : objectsJson)
            {
                std::string name = objectJson["name"].asString();
                unsigned int nameHash = Crypto::stringHash(name);
                MapEntityTemplate t;
                t.name = name;
                t.json = objectJson;
                
                std::string superName = objectJson["super"].asString();
                if (!superName.empty())
                {
                    unsigned int superHash = Crypto::stringHash(superName);
                    auto superIt = templates.find(superHash);
                    assert(superIt != templates.end() && "The super type has to be declared before its children.");
                    if (superIt != templates.end())
                    {
                        mergeTemplate(superIt->second, t);
                    }
                }
                
                t.abstract = objectJson["abstract"].asBool();
                
                templates[nameHash] = t;
            }
        }
        
        MapEntity* MapEntityFactory::createEntity(const std::string& name) const
        {
            unsigned int hash = Crypto::stringHash(name);
            for (const auto& templatesIt : templatesByFile)
            {
                const auto& templates = templatesIt.second;
                const auto it = templates.find(hash);
                if (it != templates.end())
                {
                    return createEntity(it->second);
                }
            }
            
            CCASSERT(false, ("Could not create an entity with name " + name).c_str());
            
            return nullptr;
        }
        
        MapEntity* MapEntityFactory::createEntity(const MapEntityTemplate& t) const
        {
            assert(!t.abstract && "Can't instantiate an abstract template");
            
            MapEntity* result = new MapEntity();
            
            for (const auto& componentJson : t.json["components"])
            {
                Component* component = componentFactory->createComponent(componentJson);
                if (component)
                {
                    result->addComponent(component);
                }
            }
            
            return result;
        }
    }
}