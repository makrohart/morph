#pragma  once

#include "../aspectable/Aspectable.h"
#include "MorphNode.h"

namespace morph
{
    struct MorphDivNode : MorphNode
    {
        MorphDivNode()
        {
            setBackgroundColorR(240);
            setBackgroundColorG(240);
            setBackgroundColorB(240);
            setBackgroundColorA(255);

            setBorderColorR(255);
            setBorderColorG(255);
            setBorderColorB(255);
            setBorderColorA(255);

            setHeightPercent(100);
        }

        ~MorphDivNode() override = default;

        void onRender(SDL_Renderer *renderer, int& offsetX, int& offsetY) override
        {
            YGNodeRef yogaNode = getYGNodeRef().get();

            double t = YGNodeLayoutGetTop(yogaNode);
            double r = YGNodeLayoutGetRight(yogaNode);
            double b = YGNodeLayoutGetBottom(yogaNode);
            double l = YGNodeLayoutGetLeft(yogaNode);

            double w = YGNodeLayoutGetWidth(yogaNode);
            double h = YGNodeLayoutGetHeight(yogaNode);

            double mt = YGNodeLayoutGetMargin(yogaNode, YGEdgeTop);
            double mr = YGNodeLayoutGetMargin(yogaNode, YGEdgeRight);
            double mb = YGNodeLayoutGetMargin(yogaNode, YGEdgeBottom);
            double ml = YGNodeLayoutGetMargin(yogaNode, YGEdgeLeft);

            double pt = YGNodeLayoutGetPadding(yogaNode, YGEdgeTop);
            double pr = YGNodeLayoutGetPadding(yogaNode, YGEdgeRight);
            double pb = YGNodeLayoutGetPadding(yogaNode, YGEdgeBottom);
            double pl = YGNodeLayoutGetPadding(yogaNode, YGEdgeLeft);

            offsetX += l + ml + pl;
            offsetY += t + mt + pt;

            // 绘制背景
            auto backgroundColor = getBackgroundColor();
            SDL_SetRenderDrawColor(renderer, backgroundColor[0], backgroundColor[1], backgroundColor[2], backgroundColor[3]);
            SDL_FRect rect {(float)offsetX, (float)offsetY, (float)(w - mr - ml - pr - pl), (float)(h - mt - mb - pt - pb)};
            SDL_RenderFillRect(renderer, &rect);

            // // 绘制边框
            auto borderColor = getBorderColor();
            SDL_SetRenderDrawColor(renderer, borderColor[0], borderColor[1], borderColor[2], borderColor[3]);
            SDL_RenderRect(renderer, &rect);
        }
    };
};