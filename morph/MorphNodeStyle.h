#pragma once

#include <memory>

#include "../aspectable/Aspectable.h"
#include "yoga/YGNodeStyle.h"

namespace morph
{
    struct MorphColor
    {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
    };

    struct YGNodeDeleter
    {
        void operator()(YGNodeRef ygNode)
        {
            YGNodeFree(ygNode);
        }
    };

    struct MorphNodeStyle : virtual aspectable::Aspectable<aspectable::Aspect>
    {
        MorphNodeStyle() : aspectable::Aspectable<aspectable::Aspect>(this) 
        {
            m_yogaNode = std::shared_ptr<YGNode>(YGNodeNew(), YGNodeDeleter{});
            YGNodeStyleSetWidth(m_yogaNode.get(), 100);
            YGNodeStyleSetHeight(m_yogaNode.get(), 100);
            YGNodeStyleSetFlexDirection(m_yogaNode.get(), m_flexDirection);
        }

        ~MorphNodeStyle() = default;

        using Color = uint8_t;

        std::shared_ptr<YGNode> getYGNodeRef() 
        {
            return m_yogaNode; 
        }

        double getFlex() { return m_flex; }
        void setFlex(const double flex) 
        { 
            m_flex = flex; 
            YGNodeStyleSetFlex(m_yogaNode.get(), m_flex);
        }

        double getMargin() { return m_margin; }
        void setMargin(const double margin)
        { 
            m_margin = margin; 
            YGNodeStyleSetMargin(m_yogaNode.get(), YGEdgeAll, m_margin);
        }

        int getFlexDirection() { return m_flexDirection; }
        void setFlexDirection(const int flexDirection) { m_flexDirection = (YGFlexDirection)flexDirection; }

        int getJustify() { return m_justifyContent; }
        void setJustify(const int justifyContent) { m_justifyContent = (YGJustify)justifyContent; }

        MorphColor getBackgroundColor() { return m_backgroundColor; }
        void setBackgroundColor(const MorphColor backgroundColor) { m_backgroundColor = backgroundColor; }

        MorphColor getBorderColor() { return m_borderColor; }
        void setBorderColor(const MorphColor borderColor) { m_borderColor = borderColor; }

        private:
        // Layout
        double m_flex{0.0};
        double m_margin{0.0};
        YGFlexDirection m_flexDirection{YGFlexDirectionColumn};
        YGJustify m_justifyContent{YGJustifyFlexStart};

        // Render
        MorphColor m_backgroundColor{0, 0, 0, 255};
        MorphColor m_borderColor{255, 255, 255, 255};
        
        std::shared_ptr<YGNode> m_yogaNode{nullptr};
    };
}