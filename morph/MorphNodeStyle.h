#pragma once

#include <array>
#include <functional>
#include <map>
#include <memory>
#include <string>

#include "../aspectable/Aspectable.h"
#include "yoga/YGNodeStyle.h"

namespace morph
{
    struct MorphNodeStyle : aspectable::Aspectable<aspectable::Aspect>
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
            setFlexDirection(YGFlexDirectionColumn);
            // setHeightPercent(100);
            // setWidthPercent(100);
        }

        ~MorphNodeStyle() override = default;

        void setProperty(const std::string& property, const std::string& value) 
        {
            if (auto setter =  m_setterMap.find(property); setter != m_setterMap.cend())
                (this->*setter->second)(std::stod(value));
        }

        std::shared_ptr<YGNode> getYGNodeRef() { return m_yogaNode; }

        double getHeight() { return YGNodeStyleGetHeight(m_yogaNode.get()).value; }
        void setHeight(double value) { YGNodeStyleSetHeight(m_yogaNode.get(), value); }
        double getWidth() { return YGNodeStyleGetWidth(m_yogaNode.get()).value; }
        void setWidth(double value) { YGNodeStyleSetWidth(m_yogaNode.get(), value); }
        double getMinHeight() { return YGNodeStyleGetMinHeight(m_yogaNode.get()).value; }
        void setMinHeight(double value) { YGNodeStyleSetMinHeight(m_yogaNode.get(), value); }
        double getMinWidth() { return YGNodeStyleGetMinWidth(m_yogaNode.get()).value; }
        void setMinWidth(double value) { YGNodeStyleSetMinWidth(m_yogaNode.get(), value); }
        double getMaxHeight() { return YGNodeStyleGetMaxHeight(m_yogaNode.get()).value; }
        void setMaxHeight(double value) { YGNodeStyleSetMaxHeight(m_yogaNode.get(), value); }
        double getMaxWidth() { return YGNodeStyleGetMaxWidth(m_yogaNode.get()).value; }
        void setMaxWidth(double value) { YGNodeStyleSetMaxWidth(m_yogaNode.get(), value); }

        void setHeightPercent(double value) { YGNodeStyleSetHeightPercent(m_yogaNode.get(), value); }
        void setWidthPercent(double value) { YGNodeStyleSetWidthPercent(m_yogaNode.get(), value); }

        double getMarginTop() { return YGNodeStyleGetMargin(m_yogaNode.get(), YGEdgeTop).value; }
        void setMarginTop(double value) { YGNodeStyleSetMargin(m_yogaNode.get(), YGEdgeTop, value); }
        double getMarginRight() { return YGNodeStyleGetMargin(m_yogaNode.get(), YGEdgeRight).value; }
        void setMarginRight(double value) { YGNodeStyleSetMargin(m_yogaNode.get(), YGEdgeRight, value); }
        double getMarginBottom() { return YGNodeStyleGetMargin(m_yogaNode.get(), YGEdgeBottom).value; }
        void setMarginBottom(double value) { YGNodeStyleSetMargin(m_yogaNode.get(), YGEdgeBottom, value); }
        double getMarginLeft() { return YGNodeStyleGetMargin(m_yogaNode.get(), YGEdgeLeft).value; }
        void setMarginLeft(double value) { YGNodeStyleSetMargin(m_yogaNode.get(), YGEdgeLeft, value); }

        double getPaddingTop() { return YGNodeStyleGetPadding(m_yogaNode.get(), YGEdgeTop).value; }
        void setPaddingTop(double value) { YGNodeStyleSetPadding(m_yogaNode.get(), YGEdgeTop, value); }
        double getPaddingRight() { return YGNodeStyleGetPadding(m_yogaNode.get(), YGEdgeRight).value; }
        void setPaddingRight(double value) { YGNodeStyleSetPadding(m_yogaNode.get(), YGEdgeRight, value); }
        double getPaddingBottom() { return YGNodeStyleGetPadding(m_yogaNode.get(), YGEdgeBottom).value; }
        void setPaddingBottom(double value) { YGNodeStyleSetPadding(m_yogaNode.get(), YGEdgeBottom, value); }
        double getPaddingLeft() { return YGNodeStyleGetPadding(m_yogaNode.get(), YGEdgeLeft).value; }
        void setPaddingLeft(double value) { YGNodeStyleSetPadding(m_yogaNode.get(), YGEdgeLeft, value); }
        
        double getBorderTop() { return YGNodeStyleGetBorder(m_yogaNode.get(), YGEdgeTop); }
        void setBorderTop(double value) { YGNodeStyleSetBorder(m_yogaNode.get(), YGEdgeTop, value); }
        double getBorderRight() { return YGNodeStyleGetBorder(m_yogaNode.get(), YGEdgeRight); }
        void setBorderRight(double value) { YGNodeStyleSetBorder(m_yogaNode.get(), YGEdgeRight, value); }
        double getBorderBottom() { return YGNodeStyleGetBorder(m_yogaNode.get(), YGEdgeBottom); }
        void setBorderBottom(double value) { YGNodeStyleSetBorder(m_yogaNode.get(), YGEdgeBottom, value); }
        double getBorderLeft() { return YGNodeStyleGetBorder(m_yogaNode.get(), YGEdgeLeft); }
        void setBorderLeft(double value) { YGNodeStyleSetBorder(m_yogaNode.get(), YGEdgeLeft, value); }
        
        double getFlexDirection() { return YGNodeStyleGetFlexDirection(m_yogaNode.get()); }
        void setFlexDirection(double value) { YGNodeStyleSetFlexDirection(m_yogaNode.get(), (YGFlexDirection)value); }

        double getJustifyContent() { return YGNodeStyleGetJustifyContent(m_yogaNode.get()); }
        void setJustifyContent(double value) { YGNodeStyleSetJustifyContent(m_yogaNode.get(), (YGJustify)value); }

        double getAlignItems() { return YGNodeStyleGetAlignItems(m_yogaNode.get()); }
        void setAlignItems(double value) { YGNodeStyleSetAlignItems(m_yogaNode.get(), (YGAlign)(int)value); }

        double getAlignSelf() { return YGNodeStyleGetAlignSelf(m_yogaNode.get()); }
        void setAlignSelf(double value) { YGNodeStyleSetAlignSelf(m_yogaNode.get(), (YGAlign)(int)value); }

        double getFlex() { return YGNodeStyleGetFlex(m_yogaNode.get()); }
        void setFlex(double value) { YGNodeStyleSetFlex(m_yogaNode.get(), value); }
        double getFlexGrow() { return YGNodeStyleGetFlexGrow(m_yogaNode.get()); }
        void setFlexGrow(double value) { YGNodeStyleSetFlexGrow(m_yogaNode.get(), value); }
        double getFlexShrink() { return YGNodeStyleGetFlexShrink(m_yogaNode.get()); }
        void setFlexShrink(double value) { YGNodeStyleSetFlexShrink(m_yogaNode.get(), value); }
        double getFlexBasis() { return YGNodeStyleGetFlexBasis(m_yogaNode.get()).value; }
        void setFlexBasis(double value) { YGNodeStyleSetFlexBasis(m_yogaNode.get(), value); }

        double getTop() { return YGNodeStyleGetPosition(m_yogaNode.get(), YGEdgeTop).value; }
        void setTop(double value) { YGNodeStyleSetPosition(m_yogaNode.get(), YGEdgeTop, value); }
        double getRight() { return YGNodeStyleGetPosition(m_yogaNode.get(), YGEdgeRight).value; }
        void setRight(double value) { YGNodeStyleSetPosition(m_yogaNode.get(), YGEdgeRight, value); }
        double getBottom() { return YGNodeStyleGetPosition(m_yogaNode.get(), YGEdgeBottom).value; }
        void setBottom(double value) { YGNodeStyleSetPosition(m_yogaNode.get(), YGEdgeBottom, value); }
        double getLeft() { return YGNodeStyleGetPosition(m_yogaNode.get(), YGEdgeLeft).value; }
        void setLeft(double value) { YGNodeStyleSetPosition(m_yogaNode.get(), YGEdgeLeft, value); }

        void setBackgroundColorR(double value) { m_backgroundColor[0] = value; }
        void setBackgroundColorG(double value) { m_backgroundColor[1] = value; }
        void setBackgroundColorB(double value) { m_backgroundColor[2] = value; }
        void setBackgroundColorA(double value) { m_backgroundColor[3] = value;}
        Color getBackgroundColor() { return m_backgroundColor; }
        
        void setBorderColorR(double value) { m_borderColor[0] = value; }
        void setBorderColorG(double value) { m_borderColor[1] = value; }
        void setBorderColorB(double value) { m_borderColor[2] = value; }
        void setBorderColorA(double value) { m_borderColor[3] = value;}
        Color getBorderColor() { return m_borderColor; }
        
        private:
        Color m_backgroundColor{255, 255, 255, 255};
        Color m_borderColor{0, 0, 0, 255};
        
        std::shared_ptr<YGNode> m_yogaNode{nullptr};
        const std::map<std::string, void(MorphNodeStyle::*)(double)> m_setterMap {
            {"width",  &MorphNodeStyle::setWidth},
            {"height", &MorphNodeStyle::setHeight},
            {"minWidth",  &MorphNodeStyle::setMinWidth},
            {"minHeight", &MorphNodeStyle::setMinHeight},
            {"maxWidth",  &MorphNodeStyle::setMaxWidth},
            {"maxHeight", &MorphNodeStyle::setMaxHeight},
            {"widthPercent",  &MorphNodeStyle::setWidthPercent},
            {"heightPercent", &MorphNodeStyle::setHeightPercent},
            {"marginTop",  &MorphNodeStyle::setMarginTop},
            {"marginRight",  &MorphNodeStyle::setMarginRight},
            {"marginBottom",  &MorphNodeStyle::setMarginBottom},
            {"marginLeft",  &MorphNodeStyle::setMarginLeft},
            {"paddingTop",  &MorphNodeStyle::setPaddingTop},
            {"paddingRight",  &MorphNodeStyle::setPaddingRight},
            {"paddingBottom",  &MorphNodeStyle::setPaddingBottom},
            {"paddingLeft",  &MorphNodeStyle::setPaddingLeft},
            {"borderTop",  &MorphNodeStyle::setBorderTop},
            {"borderRight",  &MorphNodeStyle::setBorderRight},
            {"borderBottom",  &MorphNodeStyle::setBorderBottom},
            {"borderLeft",  &MorphNodeStyle::setBorderLeft},
            {"flexDirection", &MorphNodeStyle::setFlexDirection},
            {"justifyContent", &MorphNodeStyle::setJustifyContent},
            {"alignItems", &MorphNodeStyle::setAlignItems},
            {"alignSelf", &MorphNodeStyle::setAlignSelf},
            {"flex", &MorphNodeStyle::setFlex},
            {"flexGrow", &MorphNodeStyle::setFlexGrow},
            {"flexShrink", &MorphNodeStyle::setFlexShrink},
            {"flexBasis", &MorphNodeStyle::setFlexBasis},
            {"top", &MorphNodeStyle::setTop},
            {"right", &MorphNodeStyle::setRight},
            {"bottom", &MorphNodeStyle::setBottom},
            {"left", &MorphNodeStyle::setLeft},
            {"backgroundColorR", &MorphNodeStyle::setBackgroundColorR},
            {"backgroundColorG", &MorphNodeStyle::setBackgroundColorG},
            {"backgroundColorB", &MorphNodeStyle::setBackgroundColorB},
            {"backgroundColorA", &MorphNodeStyle::setBackgroundColorA},
            {"BorderColorR", &MorphNodeStyle::setBorderColorR},
            {"BorderColorG", &MorphNodeStyle::setBorderColorG},
            {"BorderColorB", &MorphNodeStyle::setBorderColorB},
            {"BorderColorA", &MorphNodeStyle::setBorderColorA},
        };
    };
}