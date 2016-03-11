//
//  MapEntityFactory.hpp
//  Evolution
//
//  Created by Ricard Perez del Campo on 11/03/16.
//
//

#ifndef MapEntityFactory_hpp
#define MapEntityFactory_hpp

#include <string>
#include <map>
#include <jsoncpp/json.h>

namespace MelonGames
{
    namespace Evolution
    {
        class MapEntity;
        class ComponentFactory;
        
        struct MapEntityTemplate
        {
            std::string name;
            Json::Value json;
            bool abstract = false;
            Json::Value* getComponentJson(const std::string& componentType);
        };
        
        class MapEntityFactory
        {
        public:
            MapEntityFactory();
            ~MapEntityFactory();
            
            void addTemplatesFromFile(const std::string& filename);
            MapEntity* createEntity(const std::string& name) const;
            
        private:
            MapEntity* createEntity(const MapEntityTemplate& t) const;
            void mergeTemplate(const MapEntityTemplate& superTemplate, MapEntityTemplate& baseTemplate) const;
            
        private:
            std::map<std::string, std::map<unsigned int, MapEntityTemplate>> templatesByFile;
            ComponentFactory* componentFactory;
        };
    }
}

#endif /* MapEntityFactory_hpp */
