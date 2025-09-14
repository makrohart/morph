#include "ButtonView.h"

namespace morph
{
    ButtonView::ButtonView()
    {
        setProperty("backgroundColorR", "220");
        setProperty("backgroundColorG", "220");
        setProperty("backgroundColorB", "220");
        setProperty("backgroundColorA", "255");

        setProperty("borderColorR", "0");
        setProperty("borderColorG", "0");
        setProperty("borderColorB", "0");
        setProperty("borderColorA", "255");

        setProperty("heightPercent", "50");
    }

    void ButtonView::onRender(RendererPtr& renderer, int& offsetX, int& offsetY)
    {
        const double top = getProperty("top");
        const double left = getProperty("left");
        const double width = getProperty("width");
        const double height = getProperty("height");

        offsetX += static_cast<int>(left);
        offsetY += static_cast<int>(top);

        // Get colors
        const auto backgroundColor = getColorFromProperties(
            getProperty("backgroundColorR"), 
            getProperty("backgroundColorG"), 
            getProperty("backgroundColorB"), 
            getProperty("backgroundColorA")
        );

        const auto borderColor = getColorFromProperties(
            getProperty("borderColorR"), 
            getProperty("borderColorG"), 
            getProperty("borderColorB"), 
            getProperty("borderColorA")
        );

        // Render background and border
        renderBackgroundAndBorder(renderer, backgroundColor, borderColor, 
                                static_cast<float>(offsetX), static_cast<float>(offsetY), 
                                static_cast<float>(width), static_cast<float>(height));
    }
}