#include "TextView.h"

#include "SDL3_ttf/SDL_ttf.h"

namespace morph
{
    TextView::TextView(const std::string& text) : m_text(text) 
    {
        setProperty("heightPercent", "100");
        setProperty("widthPercent", "100");
    }

    void TextView::onRender(RendererPtr& renderer, int& offsetX, int& offsetY)
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

        // 添加调试信息

        offsetX += l + ml + pl;
        offsetY += t + mt + pt;

        TTF_Font* font = TTF_OpenFont("Arial.ttf", 24);
        if (!font)
            return;

        // 设置文字颜色
        SDL_Color color = {0, 0, 0};  // 深色文字

        // 使用 TTF_RenderText_Blended_Wrapped 渲染文本，文字可以折叠
        SDL_Surface* text_surface = TTF_RenderText_Blended_Wrapped(font, m_text.data(), m_text.size(), color, w - mr - ml - pr - pl);

        // 创建纹理
        SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);

        // 设置文字位置
        SDL_FRect rect {(float)offsetX, (float)offsetY, (float)(w - mr - ml - pr - pl), (float)(h - mt - mb - pt - pb)};
        
        SDL_RenderTexture(renderer, text_texture, NULL, &rect);
    };
}