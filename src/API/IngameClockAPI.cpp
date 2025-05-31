// IngameClockAPI.cpp

#include "IngameClockAPI.h"
#include "ingame-clock.h"

namespace
{
    class ClockAPIImpl : public IngameClockAPI::ClockAPI
    {
    public:
        void ShowClock() override
        {
            IngameClock::ClockOverlay::GetSingleton()->SetVisible(true);
        }

        void HideClock() override
        {
            IngameClock::ClockOverlay::GetSingleton()->SetVisible(false);
        }

        void ToggleClock() override
        {
            auto* overlay = IngameClock::ClockOverlay::GetSingleton();
            overlay->SetVisible(!overlay->IsVisible());
        }

        bool IsClockVisible() const override
        {
            return IngameClock::ClockOverlay::GetSingleton()->IsVisible();
        }
        void RefreshSettings() override
        {
            auto* overlay = IngameClock::ClockOverlay::GetSingleton();
            overlay->RequestRefresh();
        }
        void SetClockPosition(float x, float y, bool a_save) override 
        {
            auto* overlay = IngameClock::ClockOverlay::GetSingleton();
            overlay->SetWindowPosition(x, y, a_save);
        }
        void SetClockColor(std::string& color_text, bool a_save) override
        {
            auto* overlay = IngameClock::ClockOverlay::GetSingleton();
            overlay->SetColor(color_text, a_save);
        }

        bool HasControlsDisableExternally() override
        {
            auto* overlay = IngameClock::ClockOverlay::GetSingleton();
            return overlay->IsExternallyControlled();
        }

        void DisableVisibilityControls(std::string& mod_name, bool enable) override
        {
            auto* overlay = IngameClock::ClockOverlay::GetSingleton();
            overlay->SetExternallyControlled(enable, mod_name);
        }
        std::optional<std::string> GetControllingModName() const override
        {
            auto* overlay = IngameClock::ClockOverlay::GetSingleton();
            if (auto mod = overlay->GetExternallyControllingModName()) {
                logs::info("Clock controls are currently disabled by {}", *mod);
            } else {
                logs::info("No mod has disabled clock controls.");
            }
            return overlay->GetExternallyControllingModName();
        }

    };
    ClockAPIImpl g_clockAPI;
}

IngameClockAPI::ClockAPI* IngameClockAPI::GetAPI()
{
    return &g_clockAPI;
}

int IngameClockAPI::GetAPIVersion()
{ 
    return API_VERSION; 
} 
