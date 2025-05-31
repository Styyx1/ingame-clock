#pragma once

// Credits: Fenix Resurrection API. repo is private, so i can't link but the API is heavily based on Fenix' implementation.

namespace IngameClockAPI
{
    constexpr inline int API_VERSION = 1;

    class ClockAPI
    { 
    public:
        virtual void ShowClock() {};                     // Force show clock overlay
        virtual void HideClock() {};                       // Force hide clock overlay
        virtual void ToggleClock() {};                    // Toggle clock visibility
        virtual void RefreshSettings() {};                // Refresh Settings by loading the .toml
        virtual void SetClockPosition(float x, float y, bool a_save = true) {};               // Change Clock position
        virtual void SetClockColor(std::string& a_color, bool a_save = true) {};                  // Change Color of Clock
        virtual bool HasControlsDisableExternally() {}                                          // check if a mod has already disabled hotkey controls. It makes not a lot of sense to disable it multiple times
        virtual void DisableVisibilityControls(std::string& mod_name, bool enable = true) {};       // disable controls from the main mod in case you want to change the show/hide behavior. string is to track which mod disabled controls, call this after you get the API on kDataLoaded. this is runtime only and does not save
        virtual std::optional<std::string> GetControllingModName() const {};                    // returns the name of the mod that disabled the controls

        [[nodiscard]] virtual bool IsClockVisible() const { return true; };  // Check visibility
    protected:
        virtual ~ClockAPI() = default;
    };

    ClockAPI* GetAPI();
    int GetAPIVersion();
}

