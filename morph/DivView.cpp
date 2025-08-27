#include "DivView.h"

namespace morph
{
    DivView::DivView()
    {
        setProperty("backgroundColorR", "240");
        setProperty("backgroundColorG", "240");
        setProperty("backgroundColorB", "240");
        setProperty("backgroundColorA", "255");

        setProperty("borderColorR", "255");
        setProperty("borderColorG", "255");
        setProperty("borderColorB", "255");
        setProperty("borderColorA", "255");

        setProperty("heightPercent", "100");
    }
    
    void DivView::onRender(RendererPtr& renderer, int& offsetX, int& offsetY)
    {

        double t = getProperty("top");
        double r = getProperty("right");
        double b = getProperty("bottom");
        double l = getProperty("left");

        double w = getProperty("width");
        double h = getProperty("height");

        double mt = getProperty("marginTop");
        double mr = getProperty("marginRight");
        double mb = getProperty("marginBottom");
        double ml = getProperty("marginLeft");

        double pt = getProperty("paddingTop");
        double pr = getProperty("paddingRight");
        double pb = getProperty("paddingBottom");
        double pl = getProperty("paddingLeft");

        offsetX += l + ml + pl;
        offsetY += t + mt + pt;

        // 绘制背景
        ILayout::Color backgroundColor = { getProperty("backgroundColorR"), 
                                            getProperty("backgroundColorG"), 
                                            getProperty("backgroundColorB"), 
                                            getProperty("backgroundColorA")};

        SDL_SetRenderDrawColor(renderer, backgroundColor[0], backgroundColor[1], backgroundColor[2], backgroundColor[3]);
        SDL_FRect rect {(float)offsetX, (float)offsetY, (float)(w - mr - ml - pr - pl), (float)(h - mt - mb - pt - pb)};
        SDL_RenderFillRect(renderer, &rect);

        // // 绘制边框
        ILayout::Color borderColor = { getProperty("borderColorR"), 
                                        getProperty("borderColorG"), 
                                        getProperty("borderColorB"), 
                                        getProperty("borderColorA")};
                                        
        SDL_SetRenderDrawColor(renderer, borderColor[0], borderColor[1], borderColor[2], borderColor[3]);
        SDL_RenderRect(renderer, &rect);
    }
}