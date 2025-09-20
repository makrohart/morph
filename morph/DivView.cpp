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
        const auto [top, left, width, height] = getBoundingBox();

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
                                contentWidth, contentHeight);
    }
}