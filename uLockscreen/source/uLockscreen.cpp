#include <uLockscreen.hpp>
#include <ctime>

// Implement all the layout/application functions here

LockscreenLayout::LockscreenLayout() : Layout::Layout() 
{
    pu::ui::Color textColor = { 0xFF, 0xFF, 0xFF, 0xFF };
    pu::ui::Color backgroundColor = { 0, 0, 0, 0xFF };

    this->unlockBackground = pu::ui::elm::Rectangle::New(0,0, 1280, 720, backgroundColor);

    // Create the TextBlock instance with the text we want
    this->unlockText = pu::ui::elm::TextBlock::New(640, 240, "Press Any Button to Unlock");
    this->unlockText->SetColor(textColor);

    
    // Add the instance to the layout. IMPORTANT! this MUST be done for them to be used, having them as members is not enough (just a simple way to keep them)
    this->Add(this->unlockBackground);
    this->Add(this->unlockText);
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
        timeSinceLastPress ++;


        if (timeSinceLastPress > 250)
        {
            timeSinceLastPress = 0;
            appletStartSleepSequence(true);
        }

        localButtons currentButton = None;
        
        // If X is pressed, start with our dialog questions!
        if(keys_down & HidNpadButton_A) 
        {
            currentButton = A_Button;
        }
        if (keys_down & HidNpadButton_B)
        {
            currentButton = B_Button;
        }
        if (keys_down & HidNpadButton_X)
        {
            currentButton = X_Button;
        }
        if (keys_down & HidNpadButton_Y)
        {
            currentButton = Y_Button;
        }
        if (keys_down & HidNpadButton_StickL)
        {
            currentButton = L_Stick;
        }
        if (keys_down & HidNpadButton_StickR)
        {
            currentButton = R_Stick;
        }
        if (keys_down & HidNpadButton_L)
        {
            currentButton = L_Trigger;
        }
        if (keys_down & HidNpadButton_R)
        {
            currentButton = R_Trigger;
        }
        if (keys_down & HidNpadButton_ZL)
        {
            currentButton = ZL_Trigger;
        }
        if (keys_down & HidNpadButton_ZR)
        {
            currentButton = ZR_Trigger;
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

        if (buttonPresses > 2)
        {
            this->Close();
        }

    });
}