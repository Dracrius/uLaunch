
#pragma once

#include <pu/Plutonium>

// Define your main layout as a class inheriting from pu::Layout
class LockscreenLayout : public pu::ui::Layout {
    private:

        // An easy way to keep objects is to have them as private members
        // Using ::Ref (of a Plutonium built-in object or any class having PU_SMART_CTOR) is an alias to a shared_ptr of the instance.
        pu::ui::elm::TextBlock::Ref unlockText;
        pu::ui::elm::Rectangle::Ref unlockBackground;

    public:

        LockscreenLayout();

        // Have ::Ref alias and ::New() static constructor
        PU_SMART_CTOR(LockscreenLayout)
};

enum localButtons
{
    A_Button,
    B_Button,
    X_Button,
    Y_Button,
    L_Stick,
    R_Stick,
    L_Trigger,
    R_Trigger,
    ZL_Trigger,
    ZR_Trigger,
    None
};

// Define your application (can't instantiate base class, so need to make a derived one)
class uLockscreen : public pu::ui::Application {
    private:

        // Layout instance
        LockscreenLayout::Ref layout;
        localButtons lastButton = None;
        int buttonPresses = 0;
        int timeSinceLastPress = 0;

        int lastMin = 0;
        int lastSec = 0;
    
    public:
        using Application::Application;
        PU_SMART_CTOR(uLockscreen)

        // We need to define this, and use it to initialize everything
        void OnLoad() override;
};