#pragma once

#include "imgui.h"
#include "settings.h"


namespace IngameClock
{
    class ClockOverlay
    {
    public:
        static void Draw();
        static void SetVisible(bool visible);
        static bool IsVisible();
        static inline Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> clockTexture;
        static inline ImTextureID clockTextureID;
        static Settings::Values::RGBA GetClockColor();
        static inline Settings::Values::RGBA textColor{0xFF, 0xFF, 0xFF, 0xFF};

    private:
        inline void DrawWithImage();
        inline void DrawOnlyText();
        static void SetClockText(std::string &input);
        inline static bool visible = true;
        inline static float scale = 1.0f;
        inline static ImVec2 position = ImVec2(50, 50);
        inline static float clockTextureSizeX;
        inline static float clockTextureSizeY;
        
    };
}