#pragma once

#include <array>
#include <memory>

#include "../aspectable/Aspectable.h"
#include "yoga/YGNodeStyle.h"

namespace morph
{
    struct MorphNodeStyle : virtual aspectable::Aspectable<aspectable::Aspect>
    {
        using Margin = std::array<double, 4>;
        using Color = std::array<uint8_t, 4>;
        struct YGNodeDeleter
        {
            void operator()(YGNodeRef ygNode)
            {
                YGNodeFree(ygNode);
            }
        };
      
        MorphNodeStyle() : aspectable::Aspectable<aspectable::Aspect>(this) 
        {
            m_yogaNode = std::shared_ptr<YGNode>(YGNodeNew(), YGNodeDeleter{});
            setHeight(-1, 100);
            setWidth(-1, 50);
            setMargin(5, 5, 5, 5);
            setFlexDirection(YGFlexDirectionColumn);
        }

        ~MorphNodeStyle() = default;

        double getHeight() { return YGNodeStyleGetHeight(m_yogaNode.get()).value; }
        void setHeight(double height, double heightPercentage)
        {
            // Use absoloute height
            if (heightPercentage > 0 && heightPercentage <= 100)
            {
                YGNodeStyleSetHeightPercent(m_yogaNode.get(), heightPercentage);
                return;
            }

            YGNodeStyleSetHeight(m_yogaNode.get(), height);
        }

        double getWidth() { return YGNodeStyleGetWidth(m_yogaNode.get()).value; }
        void setWidth(double width, double widthPercentage)
        {
            // Use absoloute height
            if (widthPercentage > 0 && widthPercentage <= 100)
            {
                YGNodeStyleSetWidthPercent(m_yogaNode.get(), widthPercentage);
                return;
            }

            YGNodeStyleSetWidth(m_yogaNode.get(), width);
        }

        MorphNodeStyle::Margin getMargin()
        {
            return MorphNodeStyle::Margin{
                YGNodeStyleGetMargin(m_yogaNode.get(), YGEdgeTop).value,
                YGNodeStyleGetMargin(m_yogaNode.get(), YGEdgeRight).value,
                YGNodeStyleGetMargin(m_yogaNode.get(), YGEdgeBottom).value,
                YGNodeStyleGetMargin(m_yogaNode.get(), YGEdgeLeft).value,
            };
        }
        void setMargin(double top, double right, double bottom, double left)
        {
            YGNodeStyleSetMargin(m_yogaNode.get(), YGEdgeTop, top);
            YGNodeStyleSetMargin(m_yogaNode.get(), YGEdgeRight, right);
            YGNodeStyleSetMargin(m_yogaNode.get(), YGEdgeBottom, bottom);
            YGNodeStyleSetMargin(m_yogaNode.get(), YGEdgeLeft, left);     
        }

        int getFlexDirection() { return YGNodeStyleGetFlexDirection(m_yogaNode.get()); }
        void setFlexDirection(const int flexDirection) { YGNodeStyleSetFlexDirection(m_yogaNode.get(), (YGFlexDirection)flexDirection); }

        Color getBackgroundColor() { return m_backgroundColor; }
        void setBackgroundColor(const Color backgroundColor) { m_backgroundColor = backgroundColor; }
        Color getBorderColor() { return m_borderColor; }
        void setBorderColor(const Color borderColor) { m_borderColor = borderColor; }

        std::shared_ptr<YGNode> getYGNodeRef() { return m_yogaNode; }

        private:
        Color m_backgroundColor{255, 255, 255, 255};
        Color m_borderColor{0, 0, 0, 255};
        
        std::shared_ptr<YGNode> m_yogaNode{nullptr};
    };
}