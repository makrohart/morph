#pragma once

#include <set>
#include <string>

#include "MorphNodeStyle.h"
#include "SDL3/SDL.h"
#include "yoga/Yoga.h"
#include "yoga/node/Node.h"

namespace morph
{
    struct MorphNode : MorphNodeStyle
    {
        MorphNode();

        ~MorphNode() override;

        void add(MorphNode* pChild);

        void remove(MorphNode* pChild);

        void render(SDL_Renderer *renderer, int offsetX, int offsetY);
        virtual void onRender(SDL_Renderer *renderer, int& offsetX, int& offsetY);

        virtual MorphNode* getSelectedNode(int x, int y);

        static MorphNode* getRootNode();    
        static void setRootNode(MorphNode* node);
        
        private:
        static int getNextId()
        {
            static int s_id{0};
            return s_id++;
        }

        private:
        int m_id{MorphNode::getNextId()};
        MorphNode* m_pParent = nullptr;
        std::set<MorphNode*> m_pChildren;
    };
}