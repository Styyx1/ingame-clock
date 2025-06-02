#pragma once
#include "imgui.h"
#include <unordered_set>
// huge thanks to po3 (photo mode: https://github.com/powerof3/PhotoMode) and Ersh (open animation replacer: https://github.com/ersh1/OpenAnimationReplacer).
// everything i did with ImGui is based on their work in some way or another.

namespace IngameClock
{
	class ClockOverlay : public REX::Singleton<ClockOverlay>
    {
    public:
        void Draw();
        void SetVisible(bool visible);
        bool IsVisible() const;
        void SetEditorMode(bool active);
        bool IsEditorModeActive() const;        
        ImColor GetClockColor();
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> clockTexture;
        ImTextureID clockTextureID;
        ImColor textColor{0xFF, 0xFF, 0xFF, 0xFF};
        void FocusLost();
        void FocusRegained();
		bool Uses24HourFormat() const;
        void ManipulateSettingValues();
        void Set24HourFormat(bool activate);
		bool ShowsRealTime() const { return show_real_time; }
		bool ShowsGameTime() const { return show_game_time; }
		void SetShowRealTime(bool activate) { show_real_time = activate; }
		void SetShowGameTime(bool activate) { show_game_time = activate; }
		float GetScale() const { return scale; }
        void SetScale(float newScale, bool save);
        void SetColor(const std::string& color, bool save = false);
        void SetWindowPosition(float x, float y, bool save = false);
        void SetControlDisabler(const char* a_modName, bool a_enable);
        bool IsExternallyControlled() const;
        [[nodiscard]] std::string GetExternallyControllingModName() const;
		bool GetPositionX() const { return currentWindowPos.x; }
		bool GetPositionY() const { return currentWindowPos.y; }
        std::string GetGameTimeString();
        std::string GetRealTimeString();
        void RefreshSettings();
        void RequestRefresh();
        bool ShouldRefresh() const;

    private:
        mutable std::shared_mutex mutex;
        std::unordered_set<std::string> controlDisablers;
        void DrawEditor();
        void InitEditor();
        void DrawClock();
        void InitSettings();
        bool shouldDraw() const;
        std::string GetGameTimeText() const;
		std::string GetRealTimeText() const;
        bool editor_active = false;
        bool visible = true;
        bool force_visible = true;
        ImVec2 currentWindowPos = ImVec2(0, 0);
        bool _lostFocus = false;
		bool use24hFormat = false;
		bool show_real_time = false;
        bool show_game_time = true;
		bool settings_initialised = false;
        bool should_refresh_settings = false;
        float scale = 1.0f;
        float alpha = 1.0f;
        bool external_controls = false;
        std::string external_control_mod{""};
    };
}