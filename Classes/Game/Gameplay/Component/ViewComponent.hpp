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
#include "math/Vec2.h"
#include <vector>

namespace cocos2d
{
    class Node;
    class Sprite;
}

namespace spine
{
    class SkeletonAnimation;
}

namespace MelonGames
{
    namespace Evolution
    {
        class PositionComponent;
        class ViewPart;
        
        class ViewComponent : public Component
        {
        public:
            DECLARE_TYPE_WITH_BASE_TYPE(ViewComponent, Component);
            
            ViewComponent();
            ~ViewComponent();
            
            void onAddedToMapEntity(MapEntity* entity) override;
            void onRemovedFromMapEntity(MapEntity* entity) override;
            void update(float dt) override;
            
            void addPart(ViewPart* part, bool takeOwnership);
            void removePart(ViewPart* part);
            
        private:
            void onPositionChanged(PositionComponent* posComponent);
            
        private:
            struct ViewPartData
            {
                ViewPart* viewPart;
                bool owned;
            };
            std::vector<ViewPartData> parts;
            
            bool alreadyAdded;
        };
        
#pragma mark - ViewParts
        class ViewPart
        {
        public:
            DECLARE_BASE_TYPE(ViewPart);
            
            ViewPart();
            virtual ~ViewPart();
            
            virtual cocos2d::Node* getNode() = 0;
            
            void setPositionOffset(const cocos2d::Vec2& offset);
            const cocos2d::Vec2& getPositionOffset() const;
            
            virtual void onAddedToMapEntity(MapEntity* entity);
            virtual void onRemovedFromMapEntity(MapEntity* entity);
            
        protected:
            cocos2d::Vec2 positionOffset;
        };
        
        class ViewPartSprite : public ViewPart
        {
        public:
            DECLARE_TYPE_WITH_BASE_TYPE(ViewPartSprite, ViewPart);
            
            ViewPartSprite(const std::string& spriteFrameName);
            ~ViewPartSprite();
            
            cocos2d::Node* getNode() override;
            
        private:
            cocos2d::Sprite* sprite;
        };
        
        class ViewPartSpine : public ViewPart
        {
        public:
            DECLARE_TYPE_WITH_BASE_TYPE(ViewPartSpine, ViewPart);
            
            ViewPartSpine(const std::string& skeletonDataFile, const std::string& atlasFile, float scale);
            ~ViewPartSpine();
            
            cocos2d::Node* getNode() override;
            
        private:
            spine::SkeletonAnimation* skeletonNode;
        };
    }
}

#endif /* ViewComponent_hpp */
