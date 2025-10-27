#include "YGLayout.h"
#include "YGLayoutChangedEventArgs.h"
#include "yoga/node/Node.h"
#include "yoga/Yoga.h"

 namespace morph
 {
    YGLayout::YGLayout()
    {
        m_yogaNode = std::shared_ptr<YGNode>(YGNodeNew(), YGNodeDeleter{});
        setFlexDirection(YGFlexDirectionColumn);
    }

    YGLayout::~YGLayout()
    {
        if (m_yogaNodeParent)
            YGNodeRemoveChild(m_yogaNodeParent.get(), m_yogaNode.get());
    }

    void YGLayout::setProperty(const std::string& property, const std::string& value)
    {
        if (auto setter =  m_setterMap.find(property); setter != m_setterMap.cend())
        {
            (this->*setter->second)(std::stod(value));
            return;
        }

        throw std::runtime_error("Not supported property");
    }

    double YGLayout::getProperty(const std::string& property)
    {
        if (auto getter =  m_getterMap.find(property); getter != m_getterMap.cend())
        {
            return (this->*getter->second)();
        }
        throw std::runtime_error("Not supported property");
    }

    void YGLayout::calculate(const double width, const double height)
    {
        YGNodeCalculateLayout(m_yogaNode.get(), width, height, YGDirectionLTR);
    }

    void YGLayout::onLayoutChanged(const eventable::EventArgs& onAddedArgs)
    {
        const YGLayoutChangedEventArgs* pArgs = dynamic_cast<const YGLayoutChangedEventArgs*>(&onAddedArgs);
        if (!pArgs)
            return;

        bool bIsAdded  = pArgs->IsAdded;
        const View* pParentView = pArgs->ParentView;
        ILayout* pParentLayout = pParentView ? pParentView->getLayout() : nullptr;
        YGLayout* pParentYGLayout = pParentLayout ? dynamic_cast<YGLayout*>(pParentLayout) : nullptr;
        if (!pParentYGLayout)
            return;

        if (bIsAdded)
        {
            auto child = facebook::yoga::resolveRef(m_yogaNode.get());
            child->setOwner(nullptr);
            size_t index = pParentView->getChildrenSize();
            YGNodeInsertChild(pParentYGLayout->m_yogaNode.get(), m_yogaNode.get(), index);
        }
        else
        {
            YGNodeRemoveChild(pParentYGLayout->m_yogaNode.get(), m_yogaNode.get());
        }
    }
 }
