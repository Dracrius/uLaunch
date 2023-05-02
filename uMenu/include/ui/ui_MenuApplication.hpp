
#pragma once
#include <ui/ui_TransitionGuard.hpp>
#include <ui/ui_StartupLayout.hpp>
#include <ui/ui_MenuLayout.hpp>
#include <ui/ui_ThemeMenuLayout.hpp>
#include <ui/ui_SettingsMenuLayout.hpp>
#include <ui/ui_LanguagesMenuLayout.hpp>
#include <am/am_DaemonMessages.hpp>

namespace ui {

    // TODO: user-defined literal? ---> "ok"_lang
    std::string GetLanguageString(const std::string &name);

    enum class MenuType {
        Startup,
        Main,
        Settings,
        Theme,
        Languages
    };

    void UiOnHomeButtonDetection();

    class MenuApplication : public pu::ui::Application {
        private:
            dmi::MenuStartMode start_mode;
            StartupLayout::Ref startup_lyt;
            MenuLayout::Ref menu_lyt;
            ThemeMenuLayout::Ref theme_menu_lyt;
            SettingsMenuLayout::Ref settings_menu_lyt;
            LanguagesMenuLayout::Ref languages_menu_lyt;
            pu::ui::extras::Toast::Ref notif_toast;
            dmi::DaemonStatus daemon_status;
            MenuType loaded_menu;
            JSON ui_json;
            JSON bgm_json;
            bool bgm_loop;
            u32 bgm_fade_in_ms;
            u32 bgm_fade_out_ms;
            pu::audio::Music bgm;
            pu::ui::Color text_clr;
            pu::ui::Color menu_focus_clr;
            pu::ui::Color menu_bg_clr;
            pu::audio::Sfx logout_sfx;

        public:
            using Application::Application;

            ~MenuApplication() {
                pu::audio::DestroyMusic(this->bgm);
                pu::audio::DestroySfx(this->logout_sfx);
            }
            
            PU_SMART_CTOR(MenuApplication)

            static inline void RegisterHomeButtonDetection() {
                am::RegisterOnMessageDetect(&UiOnHomeButtonDetection, dmi::MenuMessage::HomeRequest);
            }

            void OnLoad() override;

            void PlayLogoutSfx();

            inline void SetInformation(const dmi::MenuStartMode start_mode, const dmi::DaemonStatus daemon_status, const JSON ui_json) {
                this->start_mode = start_mode;
                this->daemon_status = daemon_status;
                this->ui_json = ui_json;
            }

            inline void LoadMenu() {
                this->menu_lyt->SetUser(this->daemon_status.selected_user);
                this->LoadLayout(this->menu_lyt);
                this->loaded_menu = MenuType::Main;
            }

            inline void LoadStartupMenu() {
                this->StopPlayBGM();
                this->startup_lyt->ReloadMenu();
                this->LoadLayout(this->startup_lyt);
                this->loaded_menu = MenuType::Startup;
            }

            inline void LoadThemeMenu() {
                this->theme_menu_lyt->Reload();
                this->LoadLayout(this->theme_menu_lyt);
                this->loaded_menu = MenuType::Theme;
            }

            inline void LoadSettingsMenu() {
                this->settings_menu_lyt->Reload(true);
                this->LoadLayout(this->settings_menu_lyt);
                this->loaded_menu = MenuType::Settings;
            }

            inline void LoadSettingsLanguagesMenu() {
                this->languages_menu_lyt->Reload();
                this->LoadLayout(this->languages_menu_lyt);
                this->loaded_menu = MenuType::Languages;
            }

            inline bool IsTitleSuspended() {
                return this->daemon_status.app_id != 0;
            }

            inline bool IsHomebrewSuspended() {
                return strlen(this->daemon_status.params.nro_path) > 0;
            }

            inline bool IsSuspended() {
                return this->IsTitleSuspended() || this->IsHomebrewSuspended();
            }
            
            inline bool EqualsSuspendedHomebrewPath(const std::string &path) {
                return this->daemon_status.params.nro_path == path;
            }

            inline u64 GetSuspendedApplicationId() {
                return this->daemon_status.app_id;
            }

            inline void NotifyEndSuspended() {
                // Blanking the whole status would also blank the selected user, thus we only blank the params
                this->daemon_status.params = {};
                this->daemon_status.app_id = 0;
            }

            inline bool LaunchFailed() {
                return this->start_mode == dmi::MenuStartMode::MenuLaunchFailure;
            }

            void ShowNotification(const std::string &text, const u64 timeout = 1500);

            template<typename T>
            inline T GetUIConfigValue(const std::string &name, const T def) {
                return this->ui_json.value<T>(name, def);
            }

            template<typename Elem>
            inline void ApplyConfigForElement(const std::string &menu, const std::string &name, std::shared_ptr<Elem> &elem, const bool apply_visible = true) {
                if(this->ui_json.count(menu)) {
                    const auto menu_json = this->ui_json[menu];
                    if(menu_json.count(name)) {
                        const auto elem_json = menu_json[name];

                        auto set_coords = false;
                        if(apply_visible) {
                            const auto visible = elem_json.value("visible", true);
                            elem->SetVisible(visible);
                            set_coords = visible;
                        }
                        else {
                            set_coords = true;
                        }

                        if(set_coords) {
                            if(elem_json.count("x")) {
                                const s32 x = elem_json.value("x", 0);
                                elem->SetX(x);
                            }
                            if(elem_json.count("y")) {
                                const s32 y = elem_json.value("y", 0);
                                elem->SetY(y);
                            }
                        }
                    }
                }
            }

            inline pu::ui::Color GetTextColor() {
                return this->text_clr;
            }

            inline pu::ui::Color GetMenuFocusColor() {
                return this->menu_focus_clr;
            }

            inline pu::ui::Color GetMenuBackgroundColor() {
                return this->menu_bg_clr;
            }

            void StartPlayBGM();
            void StopPlayBGM();

            inline StartupLayout::Ref &GetStartupLayout() {
                return this->startup_lyt;
            }

            inline MenuLayout::Ref &GetMenuLayout() {
                return this->menu_lyt;
            }

            inline ThemeMenuLayout::Ref &GetThemeMenuLayout() {
                return this->theme_menu_lyt;
            }

            inline SettingsMenuLayout::Ref &GetSettingsMenuLayout() {
                return this->settings_menu_lyt;
            }
            
            inline LanguagesMenuLayout::Ref &GetLanguagesMenuLayout() {
                return this->languages_menu_lyt;
            }

            void SetSelectedUser(const AccountUid user_id);
            
            inline AccountUid GetSelectedUser() {
                return this->daemon_status.selected_user;
            }

            inline MenuType GetCurrentLoadedMenu() {
                return this->loaded_menu;
            }
    };

}