#pragma once

#include <array>
#include <map>
#include <memory>

#include "ILayout.h"
#include "yoga/YGNodeStyle.h"
#include "yoga/Yoga.h"

namespace morph
{
    struct YGLayout : ILayout
    {
        struct YGNodeDeleter
        {
            inline void operator()(YGNodeRef ygNode)
            {
                YGNodeFree(ygNode);
            }
        };
      
        YGLayout();
        ~YGLayout() override;

        void setProperty(const std::string& property, const std::string& value) override;
        double getProperty(const std::string& property) override;
        void calculate(const double width, const double height) override;
        void onLayoutChanged(const eventable::EventArgs& onAddedArgs) override;

        private:
        
        inline void setHeight(double value) { YGNodeStyleSetHeight(m_yogaNode.get(), value); }
        inline void setWidth(double value) { YGNodeStyleSetWidth(m_yogaNode.get(), value); }
        // inline double getMinHeight() { return YGNodeStyleGetMinHeight(m_yogaNode.get()).value; }
        inline void setMinHeight(double value) { YGNodeStyleSetMinHeight(m_yogaNode.get(), value); }
        // inline double getMinWidth() { return YGNodeStyleGetMinWidth(m_yogaNode.get()).value; }
        inline void setMinWidth(double value) { YGNodeStyleSetMinWidth(m_yogaNode.get(), value); }
        // inline double getMaxHeight() { return YGNodeStyleGetMaxHeight(m_yogaNode.get()).value; }
        inline void setMaxHeight(double value) { YGNodeStyleSetMaxHeight(m_yogaNode.get(), value); }
        // inline double getMaxWidth() { return YGNodeStyleGetMaxWidth(m_yogaNode.get()).value; }
        inline void setMaxWidth(double value) { YGNodeStyleSetMaxWidth(m_yogaNode.get(), value); }

        inline double getHeight() { return YGNodeLayoutGetHeight(m_yogaNode.get()); }
        inline double getWidth() { return YGNodeLayoutGetWidth(m_yogaNode.get()); }
        
        inline double getStyleHeight() { return YGNodeStyleGetHeight(m_yogaNode.get()).value; }
        inline double getStyleWidth() { return YGNodeStyleGetWidth(m_yogaNode.get()).value; }

        inline void setHeightPercent(double value) { YGNodeStyleSetHeightPercent(m_yogaNode.get(), value); }
        inline void setWidthPercent(double value) { YGNodeStyleSetWidthPercent(m_yogaNode.get(), value); }

        
        inline void setMarginTop(double value) { YGNodeStyleSetMargin(m_yogaNode.get(), YGEdgeTop, value); }     
        inline void setMarginRight(double value) { YGNodeStyleSetMargin(m_yogaNode.get(), YGEdgeRight, value); }     
        inline void setMarginBottom(double value) { YGNodeStyleSetMargin(m_yogaNode.get(), YGEdgeBottom, value); }   
        inline void setMarginLeft(double value) { YGNodeStyleSetMargin(m_yogaNode.get(), YGEdgeLeft, value); }

        inline double getMarginTop() { return YGNodeLayoutGetMargin(m_yogaNode.get(), YGEdgeTop); }
        inline double getMarginRight() { return YGNodeLayoutGetMargin(m_yogaNode.get(), YGEdgeRight); }
        inline double getMarginBottom() { return YGNodeLayoutGetMargin(m_yogaNode.get(), YGEdgeBottom); }
        inline double getMarginLeft() { return YGNodeLayoutGetMargin(m_yogaNode.get(), YGEdgeLeft); }

        inline void setPaddingTop(double value) { YGNodeStyleSetPadding(m_yogaNode.get(), YGEdgeTop, value); }
        inline void setPaddingRight(double value) { YGNodeStyleSetPadding(m_yogaNode.get(), YGEdgeRight, value); }
        inline void setPaddingBottom(double value) { YGNodeStyleSetPadding(m_yogaNode.get(), YGEdgeBottom, value); }
        inline void setPaddingLeft(double value) { YGNodeStyleSetPadding(m_yogaNode.get(), YGEdgeLeft, value); }

        inline double getPaddingTop() { return YGNodeLayoutGetPadding(m_yogaNode.get(), YGEdgeTop); }
        inline double getPaddingRight() { return YGNodeLayoutGetPadding(m_yogaNode.get(), YGEdgeRight); }
        inline double getPaddingBottom() { return YGNodeLayoutGetPadding(m_yogaNode.get(), YGEdgeBottom); }
        inline double getPaddingLeft() { return YGNodeLayoutGetPadding(m_yogaNode.get(), YGEdgeLeft); }
        
        inline double getBorderTop() { return YGNodeStyleGetBorder(m_yogaNode.get(), YGEdgeTop); }
        inline void setBorderTop(double value) { YGNodeStyleSetBorder(m_yogaNode.get(), YGEdgeTop, value); }
        inline double getBorderRight() { return YGNodeStyleGetBorder(m_yogaNode.get(), YGEdgeRight); }
        inline void setBorderRight(double value) { YGNodeStyleSetBorder(m_yogaNode.get(), YGEdgeRight, value); }
        inline double getBorderBottom() { return YGNodeStyleGetBorder(m_yogaNode.get(), YGEdgeBottom); }
        inline void setBorderBottom(double value) { YGNodeStyleSetBorder(m_yogaNode.get(), YGEdgeBottom, value); }
        inline double getBorderLeft() { return YGNodeStyleGetBorder(m_yogaNode.get(), YGEdgeLeft); }
        inline void setBorderLeft(double value) { YGNodeStyleSetBorder(m_yogaNode.get(), YGEdgeLeft, value); }
        
        inline double getFlexDirection() { return YGNodeStyleGetFlexDirection(m_yogaNode.get()); }
        inline void setFlexDirection(double value) { YGNodeStyleSetFlexDirection(m_yogaNode.get(), (YGFlexDirection)value); }

        inline double getJustifyContent() { return YGNodeStyleGetJustifyContent(m_yogaNode.get()); }
        inline void setJustifyContent(double value) { YGNodeStyleSetJustifyContent(m_yogaNode.get(), (YGJustify)value); }

        inline double getAlignItems() { return YGNodeStyleGetAlignItems(m_yogaNode.get()); }
        inline void setAlignItems(double value) { YGNodeStyleSetAlignItems(m_yogaNode.get(), (YGAlign)(int)value); }

        inline double getAlignSelf() { return YGNodeStyleGetAlignSelf(m_yogaNode.get()); }
        inline void setAlignSelf(double value) { YGNodeStyleSetAlignSelf(m_yogaNode.get(), (YGAlign)(int)value); }

        inline double getFlex() { return YGNodeStyleGetFlex(m_yogaNode.get()); }
        inline void setFlex(double value) { YGNodeStyleSetFlex(m_yogaNode.get(), value); }
        inline double getFlexGrow() { return YGNodeStyleGetFlexGrow(m_yogaNode.get()); }
        inline void setFlexGrow(double value) { YGNodeStyleSetFlexGrow(m_yogaNode.get(), value); }
        inline double getFlexShrink() { return YGNodeStyleGetFlexShrink(m_yogaNode.get()); }
        inline void setFlexShrink(double value) { YGNodeStyleSetFlexShrink(m_yogaNode.get(), value); }
        inline double getFlexBasis() { return YGNodeStyleGetFlexBasis(m_yogaNode.get()).value; }
        inline void setFlexBasis(double value) { YGNodeStyleSetFlexBasis(m_yogaNode.get(), value); }

        inline void setTop(double value) { YGNodeStyleSetPosition(m_yogaNode.get(), YGEdgeTop, value); }
        inline void setRight(double value) { YGNodeStyleSetPosition(m_yogaNode.get(), YGEdgeRight, value); }
        inline void setBottom(double value) { YGNodeStyleSetPosition(m_yogaNode.get(), YGEdgeBottom, value); }
        inline void setLeft(double value) { YGNodeStyleSetPosition(m_yogaNode.get(), YGEdgeLeft, value); }

        inline double getTop() { return YGNodeLayoutGetTop(m_yogaNode.get()); }
        inline double getRight() { return YGNodeLayoutGetRight(m_yogaNode.get()); }
        inline double getBottom() { return YGNodeLayoutGetBottom(m_yogaNode.get()); }
        inline double getLeft() { return YGNodeLayoutGetLeft(m_yogaNode.get()); }

        inline void setBackgroundColorR(double value) { m_backgroundColor[0] = value; }
        inline void setBackgroundColorG(double value) { m_backgroundColor[1] = value; }
        inline void setBackgroundColorB(double value) { m_backgroundColor[2] = value; }
        inline void setBackgroundColorA(double value) { m_backgroundColor[3] = value;}

        inline double getBackgroundColorR() { return m_backgroundColor[0]; }
        inline double getBackgroundColorG() { return m_backgroundColor[1]; }
        inline double getBackgroundColorB() { return m_backgroundColor[2]; }
        inline double getBackgroundColorA() { return m_backgroundColor[3];}
        
        inline void setBorderColorR(double value) { m_borderColor[0] = value; }
        inline void setBorderColorG(double value) { m_borderColor[1] = value; }
        inline void setBorderColorB(double value) { m_borderColor[2] = value; }
        inline void setBorderColorA(double value) { m_borderColor[3] = value;}

        inline double getBorderColorR() { return m_borderColor[0]; }
        inline double getBorderColorG() { return m_borderColor[1]; }
        inline double getBorderColorB() { return m_borderColor[2]; }
        inline double getBorderColorA() { return m_borderColor[3];}
        
        private:
        Color m_backgroundColor{255, 255, 255, 255};
        Color m_borderColor{0, 0, 0, 255};
        
        std::shared_ptr<YGNode> m_yogaNode{nullptr};
        std::shared_ptr<YGNode> m_yogaNodeParent{nullptr};
        const std::map<std::string, void(YGLayout::*)(double)> m_setterMap {
            {"width",  &YGLayout::setWidth},
            {"height", &YGLayout::setHeight},
            {"minWidth",  &YGLayout::setMinWidth},
            {"minHeight", &YGLayout::setMinHeight},
            {"maxWidth",  &YGLayout::setMaxWidth},
            {"maxHeight", &YGLayout::setMaxHeight},
            {"widthPercent",  &YGLayout::setWidthPercent},
            {"heightPercent", &YGLayout::setHeightPercent},
            {"marginTop",  &YGLayout::setMarginTop},
            {"marginRight",  &YGLayout::setMarginRight},
            {"marginBottom",  &YGLayout::setMarginBottom},
            {"marginLeft",  &YGLayout::setMarginLeft},
            {"paddingTop",  &YGLayout::setPaddingTop},
            {"paddingRight",  &YGLayout::setPaddingRight},
            {"paddingBottom",  &YGLayout::setPaddingBottom},
            {"paddingLeft",  &YGLayout::setPaddingLeft},
            {"borderTop",  &YGLayout::setBorderTop},
            {"borderRight",  &YGLayout::setBorderRight},
            {"borderBottom",  &YGLayout::setBorderBottom},
            {"borderLeft",  &YGLayout::setBorderLeft},
            {"flexDirection", &YGLayout::setFlexDirection},
            {"justifyContent", &YGLayout::setJustifyContent},
            {"alignItems", &YGLayout::setAlignItems},
            {"alignSelf", &YGLayout::setAlignSelf},
            {"flex", &YGLayout::setFlex},
            {"flexGrow", &YGLayout::setFlexGrow},
            {"flexShrink", &YGLayout::setFlexShrink},
            {"flexBasis", &YGLayout::setFlexBasis},
            {"top", &YGLayout::setTop},
            {"right", &YGLayout::setRight},
            {"bottom", &YGLayout::setBottom},
            {"left", &YGLayout::setLeft},
            {"backgroundColorR", &YGLayout::setBackgroundColorR},
            {"backgroundColorG", &YGLayout::setBackgroundColorG},
            {"backgroundColorB", &YGLayout::setBackgroundColorB},
            {"backgroundColorA", &YGLayout::setBackgroundColorA},
            {"borderColorR", &YGLayout::setBorderColorR},
            {"borderColorG", &YGLayout::setBorderColorG},
            {"borderColorB", &YGLayout::setBorderColorB},
            {"borderColorA", &YGLayout::setBorderColorA},
        };

        const std::map<std::string, double(YGLayout::*)()> m_getterMap {
            {"styleWidth",  &YGLayout::getStyleWidth},
            {"styleHeight", &YGLayout::getStyleHeight},
            {"width",  &YGLayout::getWidth},
            {"height", &YGLayout::getHeight},
            {"top", &YGLayout::getTop},
            {"right", &YGLayout::getRight},
            {"bottom", &YGLayout::getBottom},
            {"left", &YGLayout::getLeft},
            {"marginTop",  &YGLayout::getMarginTop},
            {"marginRight",  &YGLayout::getMarginRight},
            {"marginBottom",  &YGLayout::getMarginBottom},
            {"marginLeft",  &YGLayout::getMarginLeft},
            {"paddingTop",  &YGLayout::getPaddingTop},
            {"paddingRight",  &YGLayout::getPaddingRight},
            {"paddingBottom",  &YGLayout::getPaddingBottom},
            {"paddingLeft",  &YGLayout::getPaddingLeft},
            {"backgroundColorR", &YGLayout::getBackgroundColorR},
            {"backgroundColorG", &YGLayout::getBackgroundColorG},
            {"backgroundColorB", &YGLayout::getBackgroundColorB},
            {"backgroundColorA", &YGLayout::getBackgroundColorA},
            {"borderColorR", &YGLayout::getBorderColorR},
            {"borderColorG", &YGLayout::getBorderColorG},
            {"borderColorB", &YGLayout::getBorderColorB},
            {"borderColorA", &YGLayout::getBorderColorA},
        };
    };
}