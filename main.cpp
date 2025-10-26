#include <algorithm>
#include <iostream>

#include "aspectable/Aspectable.h"
#include "journal/Journal.h"
#include "bridge/JSEngine.h"
#include "bridge/v8/V8Bridge.h"
#include "bridge/v8/V8Engine.h"
#include "needle/Sewable.h"
#include "needle/FixedString.h"
#include "add-ins/ViewModel.h"
#include "morph/ButtonView.h"
#include "morph/DivView.h"
#include "morph/TextView.h"
#include "morph/View.h"
#include "morph/WindowView.h"
#include "morph/WindowMgr.h"
#include "morph/MorphTimer.h"

struct A : public aspectable::Aspectable<aspectable::Aspect>
{
    A() : aspectable::Aspectable<aspectable::Aspect>(this) {}

    int getIntField() { return m_intField; }
    void setIntField(const int value) { m_intField = value; }

    int run(const int) 
    { 
        journal::Journal<journal::Severity::Info>() << "A::run";
        return 1; 
    }

    int m_intField;
};

int main(int argc, const char* argv[])
{
    needle::Sewable<"A", nullptr, A>().accept<V8Bridge>();
    needle::Sewable<"m_intField", &A::m_intField, decltype(&A::m_intField)>().accept<V8Bridge>();
    needle::Sewable<"run", &A::run, decltype(&A::run)>().accept<V8Bridge>();
    needle::Sewable<"IntField", &A::getIntField, decltype(&A::getIntField), needle::Property>().accept<V8Bridge>();
    needle::Sewable<"IntField", &A::setIntField, decltype(&A::setIntField), needle::Property>().accept<V8Bridge>();

    needle::Sewable<"Journal", nullptr, journal::Journal<journal::Severity::Info>>().accept<V8Bridge>();
    // TODO: aspectable not support const for referenced arguement
    needle::Sewable<"log", &journal::Journal<journal::Severity::Info>::log, decltype(&journal::Journal<journal::Severity::Info>::log)>().accept<V8Bridge>();

    needle::Sewable<"ViewModel", nullptr, mvvm::ViewModel>().accept<V8Bridge>();
    needle::Sewable<"onPropertyChanged", &mvvm::ViewModel::onPropertyChanged, decltype(&mvvm::ViewModel::onPropertyChanged)>().accept<V8Bridge>();
    needle::Sewable<"notifyPropertyChanged", &mvvm::ViewModel::notifyPropertyChanged, decltype(&mvvm::ViewModel::notifyPropertyChanged)>().accept<V8Bridge>();
    needle::Sewable<"IntField", &mvvm::ViewModel::getIntField, decltype(&mvvm::ViewModel::getIntField), needle::Property>().accept<V8Bridge>();
    needle::Sewable<"IntField", &mvvm::ViewModel::setIntField, decltype(&mvvm::ViewModel::setIntField), needle::Property>().accept<V8Bridge>();
    needle::Sewable<"StringField", &mvvm::ViewModel::getStringField, decltype(&mvvm::ViewModel::getStringField), needle::Property>().accept<V8Bridge>();
    needle::Sewable<"StringField", &mvvm::ViewModel::setStringField, decltype(&mvvm::ViewModel::setStringField), needle::Property>().accept<V8Bridge>();
    needle::Sewable<"addInt", &mvvm::ViewModel::addInt, decltype(&mvvm::ViewModel::addInt)>().accept<V8Bridge>();

    needle::Sewable<"View", nullptr, morph::View>().accept<V8Bridge>();
    needle::Sewable<"addTo", &morph::View::addTo, decltype(&morph::View::addTo)>().accept<V8Bridge>();
    needle::Sewable<"removeFrom", &morph::View::removeFrom, decltype(&morph::View::removeFrom)>().accept<V8Bridge>();
    needle::Sewable<"setProperty", &morph::View::setProperty, decltype(&morph::View::setProperty)>().accept<V8Bridge>();

    needle::Sewable<"DivView", nullptr, morph::DivView>().accept<V8Bridge>();
    needle::Sewable<"addTo", &morph::DivView::addTo, decltype(&morph::DivView::addTo)>().accept<V8Bridge>();
    needle::Sewable<"removeFrom", &morph::DivView::removeFrom, decltype(&morph::DivView::removeFrom)>().accept<V8Bridge>();
    needle::Sewable<"setProperty", &morph::DivView::setProperty, decltype(&morph::DivView::setProperty)>().accept<V8Bridge>();

    needle::Sewable<"WindowView", nullptr, morph::WindowView>().accept<V8Bridge>();
    needle::Sewable<"addTo", &morph::WindowView::addTo, decltype(&morph::WindowView::addTo)>().accept<V8Bridge>();
    needle::Sewable<"removeFrom", &morph::WindowView::removeFrom, decltype(&morph::WindowView::removeFrom)>().accept<V8Bridge>();
    needle::Sewable<"setProperty", &morph::WindowView::setProperty, decltype(&morph::WindowView::setProperty)>().accept<V8Bridge>();

    needle::Sewable<"ButtonView", nullptr, morph::ButtonView>().accept<V8Bridge>();
    needle::Sewable<"addTo", &morph::ButtonView::addTo, decltype(&morph::ButtonView::addTo)>().accept<V8Bridge>();
    needle::Sewable<"removeFrom", &morph::ButtonView::removeFrom, decltype(&morph::ButtonView::removeFrom)>().accept<V8Bridge>();
    needle::Sewable<"setProperty", &morph::ButtonView::setProperty, decltype(&morph::ButtonView::setProperty)>().accept<V8Bridge>();
    needle::Sewable<"onEvent", &morph::ButtonView::onEvent, decltype(&morph::ButtonView::onEvent)>().accept<V8Bridge>();
    needle::Sewable<"removeEvent", &morph::ButtonView::removeEvent, decltype(&morph::ButtonView::removeEvent)>().accept<V8Bridge>();

    needle::Sewable<"TextView", nullptr, morph::TextView, const std::string&>().accept<V8Bridge>();
    needle::Sewable<"addTo", &morph::TextView::addTo, decltype(&morph::TextView::addTo)>().accept<V8Bridge>();
    needle::Sewable<"removeFrom", &morph::TextView::removeFrom, decltype(&morph::TextView::removeFrom)>().accept<V8Bridge>();
    needle::Sewable<"setProperty", &morph::TextView::setProperty, decltype(&morph::TextView::setProperty)>().accept<V8Bridge>();
    needle::Sewable<"onEvent", &morph::TextView::onEvent, decltype(&morph::TextView::onEvent)>().accept<V8Bridge>();
    needle::Sewable<"setText", &morph::TextView::setText, decltype(&morph::TextView::setText)>().accept<V8Bridge>();
    needle::Sewable<"removeEvent", &morph::TextView::removeEvent, decltype(&morph::TextView::removeEvent)>().accept<V8Bridge>();


    needle::Sewable<"MorphTimer", nullptr, morph::MorphTimer>().accept<V8Bridge>();
    needle::Sewable<"setTimeout", &morph::MorphTimer::setTimeout, decltype(&morph::MorphTimer::setTimeout)>().accept<V8Bridge>();
    needle::Sewable<"clearTimeout", &morph::MorphTimer::clearTimeout, decltype(&morph::MorphTimer::clearTimeout)>().accept<V8Bridge>();

    journal::Journal<journal::Severity::Info>()<< "Hello " << "morph";
    journal::Journal<journal::Severity::Fatal>()<< "Hello " << "morph";

    try
    {
        std::unique_ptr<JSEngine> engine = std::make_unique<V8Engine>();
        
        // Order matters - load JavaScript files
        const std::vector<std::string> scripts = { "D:/Projects/morph/morphling/dist/bundle.dev.js" };
        
        engine->run(scripts);
        std::cout << "JavaScript engine initialized successfully!\n";

        // Create window manager
        morph::WindowMgr& windowMgr = morph::WindowMgr::getInstance();
        
        // Run the main loop
        windowMgr.run();
    }
    catch (const std::exception& e)
    {
        journal::Journal<journal::Severity::Fatal>() << "Error: " << e.what();
        return 1;
    }

    return 0;
}
