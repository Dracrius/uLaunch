
#pragma once

#include <pu/Plutonium>
#include <cfg/cfg_Config.hpp>

namespace ui {
    // Define your main layout as a class inheriting from pu::Layout
    class LockscreenLayout : public pu::ui::Layout {
    private:
        // An easy way to keep objects is to have them as private members
        // Using ::Ref (of a Plutonium built-in object or any class having PU_SMART_CTOR) is an alias to a shared_ptr of the instance.
        pu::ui::elm::TextBlock::Ref unlockText;
        pu::ui::elm::Rectangle::Ref unlockBackground;

    public:
        pu::ui::elm::Image::Ref unlockImg;

        pu::audio::Sfx default_unlock_sfx; //Main Unlock Sound
        pu::audio::Sfx drum_unlock_sfx; //ZL Unlock Sound
        pu::audio::Sfx horn_unlock_sfx; //ZR Unlock Sound
        pu::audio::Sfx boing_unlock_sfx; //LStick Unlock Sound
        pu::audio::Sfx owie_unlock_sfx; //RStick Unlock Sound

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
}