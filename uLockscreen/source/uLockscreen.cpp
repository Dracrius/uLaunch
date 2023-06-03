#include <uLockscreen.hpp>
#include <ctime>

//extern cfg::Config g_Config;
//extern cfg::Theme g_Theme;

// Implement all the layout/application functions here

namespace ui {
    LockscreenLayout::LockscreenLayout() : Layout::Layout()
    {
        //pu::ui::Color textColor = { 0xFF, 0xFF, 0xFF, 0xFF };
        pu::ui::Color backgroundColor = { 0, 0, 0, 0xFF };

        this->unlockBackground = pu::ui::elm::Rectangle::New(0, 0, 1280, 720, backgroundColor);
        this->Add(this->unlockBackground);
        
        this->unlockImg = pu::ui::elm::Image::New(501, 283, "ui/UnlockNone.png");
        //this->unlockImg = pu::ui::elm::Image::New(640, 240, cfg::GetAssetByTheme(g_Theme, "ui/UnlockNone.png"));
        this->Add(this->unlockImg);

        // Create the TextBlock instance with the text we want
        /*this->unlockText = pu::ui::elm::TextBlock::New(640, 240, "Press Any Button to Unlock");
        this->unlockText->SetColor(textColor);
        this->Add(this->unlockText);*/

        this->default_unlock_sfx = pu::audio::LoadSfx("sound/DefaultUnlock.wav");
        this->drum_unlock_sfx = pu::audio::LoadSfx("sound/DrumUnlock.wav");
        this->horn_unlock_sfx = pu::audio::LoadSfx("sound/HornUnlock.wav");
        this->boing_unlock_sfx = pu::audio::LoadSfx("sound/BoingUnlock.wav");
        this->owie_unlock_sfx = pu::audio::LoadSfx("sound/OwieUnlock.wav");
        /*this->default_unlock_sfx = pu::audio::LoadSfx(cfg::GetAssetByTheme(g_Theme, "sound/DefaultUnlock.wav"));
        this->drum_unlock_sfx = pu::audio::LoadSfx(cfg::GetAssetByTheme(g_Theme, "sound/DrumUnlock.wav"));
        this->horn_unlock_sfx = pu::audio::LoadSfx(cfg::GetAssetByTheme(g_Theme, "sound/HornUnlock.wav"));
        this->boing_unlock_sfx = pu::audio::LoadSfx(cfg::GetAssetByTheme(g_Theme, "sound/BoingUnlock.wav"));
        this->owie_unlock_sfx = pu::audio::LoadSfx(cfg::GetAssetByTheme(g_Theme, "sound/OwieUnlock.wav"));*/
    }

    void uLockscreen::OnLoad() {
        // Create the layout (calling the smart constructor above)
        this->layout = LockscreenLayout::New();

        // Load the layout. In applications layouts are loaded, not added into a container (you don't select an added layout, just load it from this function)
        // Simply explained: loading layout = the application will render that layout in the very next frame
        this->LoadLayout(this->layout);

        // Set a function when input is caught. This input handling will be the first one to be handled (before Layout or any Elements)
        // Using a lambda function here to simplify things
        // You can use member functions via std::bind() C++ wrapper
        this->SetOnInput([&](const u64 keys_down, const u64 keys_up, const u64 keys_held, const pu::ui::TouchPoint touch_pos)
            {
                timeSinceLastPress++;


                if (timeSinceLastPress > 250)
                {
                    timeSinceLastPress = 0;
                    appletStartSleepSequence(true);
                }

                localButtons currentButton = None;

                // If X is pressed, start with our dialog questions!
                if (keys_down & HidNpadButton_A)
                {
                    currentButton = A_Button;
                    pu::audio::PlaySfx(layout->default_unlock_sfx);
                }
                if (keys_down & HidNpadButton_B)
                {
                    currentButton = B_Button;
                    pu::audio::PlaySfx(layout->default_unlock_sfx);
                }
                if (keys_down & HidNpadButton_X)
                {
                    currentButton = X_Button;
                    pu::audio::PlaySfx(layout->default_unlock_sfx);
                }
                if (keys_down & HidNpadButton_Y)
                {
                    currentButton = Y_Button;
                    pu::audio::PlaySfx(layout->default_unlock_sfx);
                }
                if (keys_down & HidNpadButton_StickL)
                {
                    currentButton = L_Stick;
                    pu::audio::PlaySfx(layout->boing_unlock_sfx);
                }
                if (keys_down & HidNpadButton_StickR)
                {
                    currentButton = R_Stick;
                    pu::audio::PlaySfx(layout->owie_unlock_sfx);
                }
                if (keys_down & HidNpadButton_L)
                {
                    currentButton = L_Trigger;
                    pu::audio::PlaySfx(layout->default_unlock_sfx);
                }
                if (keys_down & HidNpadButton_R)
                {
                    currentButton = R_Trigger;
                    pu::audio::PlaySfx(layout->default_unlock_sfx);
                }
                if (keys_down & HidNpadButton_ZL)
                {
                    currentButton = ZL_Trigger;
                    pu::audio::PlaySfx(layout->drum_unlock_sfx);
                }
                if (keys_down & HidNpadButton_ZR)
                {
                    currentButton = ZR_Trigger;
                    pu::audio::PlaySfx(layout->horn_unlock_sfx);

                }


                if ((lastButton == currentButton && currentButton != None) || (lastButton == None && currentButton != None))
                {
                    buttonPresses++;
                    lastButton = currentButton;
                    timeSinceLastPress = 0;
                }

                if (currentButton != None && lastButton != currentButton)
                {
                    buttonPresses = 0;
                    lastButton = currentButton;
                    timeSinceLastPress = 0;
                }

                switch (buttonPresses)
                {
                case 0:
                    layout->unlockImg->SetImage("ui/UnlockNone.png");
                    //this->unlockImg->SetImage(cfg::GetAssetByTheme(g_Theme, "ui/UnlockNone.png"));
                    break;
                case 1:
                    layout->unlockImg->SetImage("ui/UnlockOne.png");
                    //this->unlockImg->SetImage(cfg::GetAssetByTheme(g_Theme, "ui/UnlockOne.png"));
                    break;
                case 2:
                    layout->unlockImg->SetImage("ui/UnlockTwo.png");
                    //this->unlockImg->SetImage(cfg::GetAssetByTheme(g_Theme, "ui/UnlockTwo.png"));
                    break;
                case 3:
                    layout->unlockImg->SetImage("ui/UnlockFull.png");
                    //this->unlockImg->SetImage(cfg::GetAssetByTheme(g_Theme, "ui/UnlockFull.png"));
                    break;
                }


                if (buttonPresses > 2 && timeSinceLastPress > 10)
                {
                    this->Close();
                }

            });
    }
}