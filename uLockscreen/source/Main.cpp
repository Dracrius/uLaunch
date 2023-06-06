#include <uLockscreen.hpp>
#include <cfg/cfg_Config.hpp>

/*
// If you would like to initialize and finalize stuff before or after Plutonium, you can use libnx's userAppInit/userAppExit

extern "C" void userAppInit() {
    // Initialize stuff
}

extern "C" void userAppExit() {
    // Cleanup/finalize stuff
}
*/
#define UL_MENU_ROMFS_BIN UL_BASE_SD_DIR "/bin/uMenu/romfs.bin"

cfg::Config g_Config;
cfg::Theme g_Theme;

namespace {

    void Initialize() 
    {
        // Load menu config and theme
        g_Config = cfg::LoadConfig();
        std::string theme_name;
        UL_ASSERT_TRUE(g_Config.GetEntry(cfg::ConfigEntryId::ActiveThemeName, theme_name));
        g_Theme = cfg::LoadTheme(theme_name);
    }
}

// Main entrypoint
int main() {
    // Check if our RomFs data exists...
    if (!fs::ExistsFile(UL_MENU_ROMFS_BIN)) {
        UL_RC_ASSERT(menu::ResultRomfsFileNotFound);
    }

    // Try to mount it
    UL_RC_ASSERT(romfsMountFromFsdev(UL_MENU_ROMFS_BIN, 0, "romfs"));

    // After initializing RomFs, start initializing the rest of stuff here
    Initialize();

    // First create our renderer, where one can customize SDL or other stuff's initialization.
    auto renderer_opts = pu::ui::render::RendererInitOptions(SDL_INIT_EVERYTHING, pu::ui::render::RendererHardwareFlags);
    renderer_opts.UseImage(pu::ui::render::IMGAllFlags);
    renderer_opts.UseAudio(pu::ui::render::MixerAllFlags);
    renderer_opts.UseTTF();
    auto renderer = pu::ui::render::Renderer::New(renderer_opts);

    // Create our main application from the renderer
    auto main = ui::uLockscreen::New(renderer);

    // Prepare out application. This MUST be called or Show() will exit and nothing will be rendered.
    main->Prepare();

    // Show -> start rendering in an "infinite" loop
    // If wou would like to show with a "fade in" from black-screen to the UI, use instead ->ShowWithFadeIn();
    main->Show();

    // Exit homebrew (Plutonium will handle all disposing of UI and renderer/application, don't worry!

    // Exit RomFs manually, since we also initialized it manually
    romfsExit();

    return 0;
}