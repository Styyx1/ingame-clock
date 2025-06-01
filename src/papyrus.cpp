#include "papyrus.h"

namespace PapyrusFunctions
{
    bool Bind(RE::BSScript::Internal::VirtualMachine* a_vm)
    {
        if (!a_vm) {
            logs::critical("couldn't get VM State"sv);
            return false;
        }

        logs::info("{:*^30}", "FUNCTIONS"sv);
        Functions::Bind(*a_vm);
        return true;
    }

    namespace Functions
    {
        int GetVersion(SCRIPT_ARGS)
        {
            return Version::kVersion;
        }

        void ToggleClock(SCRIPT_ARGS)
        {
            if (auto overlay = IngameClock::ClockOverlay::GetSingleton()) {
                overlay->SetVisible(!overlay->IsVisible());
            } else {
                logs::error("IngameClock::ClockOverlay is not initialized"sv);
            }
        }

        void HideClock(SCRIPT_ARGS)
        {
            if (auto overlay = IngameClock::ClockOverlay::GetSingleton()) {
                overlay->SetVisible(false);
            } else {
                logs::error("IngameClock::ClockOverlay is not initialized"sv);
            }
        }

        void ShowClock(SCRIPT_ARGS)
        {
            if (auto overlay = IngameClock::ClockOverlay::GetSingleton()) {
                overlay->SetVisible(true);
            } else {
                logs::error("IngameClock::ClockOverlay is not initialized"sv);
            }
        }

        bool IsClockVisible(SCRIPT_ARGS)
        {
            if (auto overlay = IngameClock::ClockOverlay::GetSingleton()) {
                return overlay->IsVisible();
            } else {
                logs::error("IngameClock::ClockOverlay is not initialized"sv);
                return false;
            }
        }

        void SetUse24hFormat(SCRIPT_ARGS, bool a_use24hFormat)
        {
            if (auto overlay = IngameClock::ClockOverlay::GetSingleton()) {
                overlay->Set24HourFormat(a_use24hFormat);
                if (const auto settings = Settings::Manager::GetSingleton()) {
                    settings->Save();
                }
            } else {
                logs::error("IngameClock::ClockOverlay is not initialized"sv);
            }
        }

        void SetShowRealTime(SCRIPT_ARGS, bool a_showRealTime)
        {
            if (auto overlay = IngameClock::ClockOverlay::GetSingleton()) {
                overlay->SetShowRealTime(a_showRealTime);
                if (const auto settings = Settings::Manager::GetSingleton()) {
                    settings->Save();
                }
            } else {
                logs::error("IngameClock::ClockOverlay is not initialized"sv);
            }
        }

        void SetShowGameTime(SCRIPT_ARGS, bool a_showGameTime)
        {
            if (auto overlay = IngameClock::ClockOverlay::GetSingleton()) {
                overlay->SetShowGameTime(a_showGameTime);
                if (const auto settings = Settings::Manager::GetSingleton()) {
                    settings->Save();
                }
            } else {
                logs::error("IngameClock::ClockOverlay is not initialized"sv);
            }
        }

        float GetScale(SCRIPT_ARGS)
        {
            if (auto overlay = IngameClock::ClockOverlay::GetSingleton()) {
                return overlay->GetScale();
            } else {
                logs::error("IngameClock::ClockOverlay is not initialized"sv);
                return 1.0f;
            }
        }

        void SetScale(SCRIPT_ARGS, float a_scale)
        {
            if (auto overlay = IngameClock::ClockOverlay::GetSingleton()) {
                overlay->SetScale(a_scale, true);
            } else {
                logs::error("IngameClock::ClockOverlay is not initialized"sv);
            }
        }

        void SetColor(SCRIPT_ARGS, RE::BSFixedString a_color)
        {
            if (auto overlay = IngameClock::ClockOverlay::GetSingleton()) {
                overlay->SetColor(a_color.c_str(), true);
            } else {
                logs::error("IngameClock::ClockOverlay is not initialized"sv);
            }
        }

        void SetPosition(SCRIPT_ARGS, float a_x, float a_y)
        {
            if (auto overlay = IngameClock::ClockOverlay::GetSingleton()) {
                overlay->SetWindowPosition(a_x, a_y, true);
            } else {
                logs::error("IngameClock::ClockOverlay is not initialized"sv);
            }
        }

        float GetPositionX(SCRIPT_ARGS)
        {
            auto settings = Settings::Manager::GetSingleton();
            if (auto overlay = IngameClock::ClockOverlay::GetSingleton()) {
                return overlay->GetPositionX();
            } else {
                logs::error("IngameClock::ClockOverlay is not initialized"sv);
                return settings->clock_position_x.GetValue();
            }
        }

        float GetPositionY(SCRIPT_ARGS)
        {
            auto settings = Settings::Manager::GetSingleton();
            if (auto overlay = IngameClock::ClockOverlay::GetSingleton()) {
                return overlay->GetPositionY();
            } else {
                logs::error("IngameClock::ClockOverlay is not initialized"sv);
                return settings->clock_position_y.GetValue();
            }
        }

        RE::BSFixedString GetGameTimeText(SCRIPT_ARGS)
        {
            if (auto overlay = IngameClock::ClockOverlay::GetSingleton()) {
                return overlay->GetGameTimeString().c_str();
            } else {
                logs::error("IngameClock::ClockOverlay is not initialized"sv);
                return "00:00";
            }
        }

        RE::BSFixedString GetRealTimeText(SCRIPT_ARGS)
        {
            if (auto overlay = IngameClock::ClockOverlay::GetSingleton()) {
                return overlay->GetRealTimeString();
            } else {
                logs::error("IngameClock::ClockOverlay is not initialized"sv);
                return "00:00";
            }
        }

        void Bind(RE::BSScript::Internal::VirtualMachine& a_vm)
        {
            constexpr auto script = "Styyx_IngameClock"sv;

            a_vm.RegisterFunction("GetVersion", script, GetVersion, true);
            a_vm.RegisterFunction("ToggleClock", script, ToggleClock);
            a_vm.RegisterFunction("HideClock", script, HideClock);
            a_vm.RegisterFunction("ShowClock", script, ShowClock);
            a_vm.RegisterFunction("IsClockVisible", script, IsClockVisible);
            a_vm.RegisterFunction("SetUse24hFormat", script, SetUse24hFormat);
            a_vm.RegisterFunction("SetShowRealTime", script, SetShowRealTime);
            a_vm.RegisterFunction("SetShowGameTime", script, SetShowGameTime);
            a_vm.RegisterFunction("GetScale", script, GetScale);
            a_vm.RegisterFunction("SetScale", script, SetScale);
            a_vm.RegisterFunction("SetColor", script, SetColor);
            a_vm.RegisterFunction("SetPosition", script, SetPosition);
            a_vm.RegisterFunction("GetPositionX", script, GetPositionX);
            a_vm.RegisterFunction("GetPositionY", script, GetPositionY);
            a_vm.RegisterFunction("GetGameTimeText", script, GetGameTimeText);
            a_vm.RegisterFunction("GetRealTimeText", script, GetRealTimeText);

            logs::info("Registered Papyrus functions for Styyx_IngameClock"sv);
        }
    }
}
