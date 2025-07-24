#pragma  once

#include "../aspectable/Aspectable.h"
#include "MorphNode.h"

namespace morph
{
    struct MorphButtonNode : MorphNode
    {
        MorphButtonNode()
        {
            setBackgroundColorR(220);
            setBackgroundColorG(220);
            setBackgroundColorB(220);
            setBackgroundColorA(255);

            setBorderColorR(0);
            setBorderColorG(0);
            setBorderColorB(0);
            setBorderColorA(255);

            setHeightPercent(50);
        }

        ~MorphButtonNode() override = default;

        void onRender(SDL_Renderer *renderer, int& offsetX, int& offsetY) override
        {
            YGNodeRef yogaNode = getYGNodeRef().get();

            double l = YGNodeLayoutGetLeft(yogaNode);
            double t = YGNodeLayoutGetTop(yogaNode);
            double r = YGNodeLayoutGetRight(yogaNode);
            double b = YGNodeLayoutGetBottom(yogaNode);
            double w = YGNodeLayoutGetWidth(yogaNode);
            double h = YGNodeLayoutGetHeight(yogaNode);

            offsetX += l;
            offsetY += t;

            // 绘制背景
            auto backgroundColor = getBackgroundColor();
            SDL_SetRenderDrawColor(renderer, backgroundColor[0], backgroundColor[1], backgroundColor[2], backgroundColor[3]);
            SDL_FRect rect {(float)offsetX, (float)offsetY, (float)w, (float)h};
            SDL_RenderFillRect(renderer, &rect);

            // // 绘制边框
            auto borderColor = getBorderColor();
            SDL_SetRenderDrawColor(renderer, borderColor[0], borderColor[1], borderColor[2], borderColor[3]);
            SDL_RenderRect(renderer, &rect);
        };
    };
};