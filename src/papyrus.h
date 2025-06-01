#pragma once

#include "RE/Skyrim.h"
#include "ingame-clock.h"
#include "settings.h"

#define SCRIPT_ARGS RE::BSScript::Internal::VirtualMachine*, RE::VMStackID, RE::StaticFunctionTag*

namespace PapyrusFunctions
{
    bool Bind(RE::BSScript::Internal::VirtualMachine* a_vm);

    namespace Functions
    {
        enum Version
        {
            kVersion = 1
        };
        int GetVersion(SCRIPT_ARGS);
        void ToggleClock(SCRIPT_ARGS);
        void HideClock(SCRIPT_ARGS);
        void ShowClock(SCRIPT_ARGS);
        bool IsClockVisible(SCRIPT_ARGS);
        void SetUse24hFormat(SCRIPT_ARGS, bool a_use24hFormat);
        void SetShowRealTime(SCRIPT_ARGS, bool a_showRealTime);
        void SetShowGameTime(SCRIPT_ARGS, bool a_showGameTime);
        float GetScale(SCRIPT_ARGS);
        void SetScale(SCRIPT_ARGS, float a_scale);
        void SetColor(SCRIPT_ARGS, RE::BSFixedString a_color);
        void SetPosition(SCRIPT_ARGS, float a_x, float a_y);
        float GetPositionX(SCRIPT_ARGS);
        float GetPositionY(SCRIPT_ARGS);
        RE::BSFixedString GetGameTimeText(SCRIPT_ARGS);
        RE::BSFixedString GetRealTimeText(SCRIPT_ARGS);
        void Bind(RE::BSScript::Internal::VirtualMachine& a_vm);
    }
}