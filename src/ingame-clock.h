#pragma once
#include "imgui.h"


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
        void RefreshVisibility();

    private:
        void SetClockText(std::string& input);
        void DrawEditor();
        void InitEditor();
        void DrawClock();
        void InitSettings();
        std::string GetGameTimeText() const;
		std::string GetRealTimeText() const;
        bool editor_active = false;
        bool visible = true;
        ImVec2 currentWindowPos = ImVec2(0, 0);
        bool _lostFocus = false;
		bool use24hFormat = false;
		bool show_real_time = false;
        bool show_game_time = true;
		bool settings_initialised = false;        
        float scale = 1.0f;
        float alpha = 1.0f;
    };
}
