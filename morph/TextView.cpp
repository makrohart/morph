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
        const double top = getProperty("top");
        const double left = getProperty("left");
        const double width = getProperty("width");
        const double height = getProperty("height");

        const double marginTop = getProperty("marginTop");
        const double marginRight = getProperty("marginRight");
        const double marginBottom = getProperty("marginBottom");
        const double marginLeft = getProperty("marginLeft");

        const double paddingTop = getProperty("paddingTop");
        const double paddingRight = getProperty("paddingRight");
        const double paddingBottom = getProperty("paddingBottom");
        const double paddingLeft = getProperty("paddingLeft");

        offsetX += static_cast<int>(left + marginLeft + paddingLeft);
        offsetY += static_cast<int>(top + marginTop + paddingTop);

        // Calculate content area
        const float contentWidth = static_cast<float>(width - marginRight - marginLeft - paddingRight - paddingLeft);
        const float contentHeight = static_cast<float>(height - marginTop - marginBottom - paddingTop - paddingBottom);

        TTF_Font* font = TTF_OpenFont("Arial.ttf", 24);
        if (!font)
            return;

        // RAII wrapper for font
        struct FontDeleter {
            void operator()(TTF_Font* f) { TTF_CloseFont(f); }
        };
        std::unique_ptr<TTF_Font, FontDeleter> fontPtr(font);

        // Set text color
        SDL_Color color = {0, 0, 0, 255};  // Black text

        // Render text with wrapping
        SDL_Surface* text_surface = TTF_RenderText_Blended_Wrapped(font, m_text.c_str(), static_cast<Uint32>(m_text.length()), color, static_cast<Uint32>(contentWidth));
        if (!text_surface)
            return;

        // RAII wrapper for surface
        struct SurfaceDeleter {
            void operator()(SDL_Surface* s) { SDL_DestroySurface(s); }
        };
        std::unique_ptr<SDL_Surface, SurfaceDeleter> surfacePtr(text_surface);

        // Create texture
        SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
        if (!text_texture)
            return;

        // RAII wrapper for texture
        struct TextureDeleter {
            void operator()(SDL_Texture* t) { SDL_DestroyTexture(t); }
        };
        std::unique_ptr<SDL_Texture, TextureDeleter> texturePtr(text_texture);

        // Set text position and render
        SDL_FRect rect{static_cast<float>(offsetX), static_cast<float>(offsetY), contentWidth, contentHeight};
        SDL_RenderTexture(renderer, text_texture, nullptr, &rect);
    }
}